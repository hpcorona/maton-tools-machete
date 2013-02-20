//
//  openal.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/1/12.
//  Copyright (c) 2012 Matón Games. All rights reserved.
//

#include "openal.h"

namespace machete {
  namespace sound {
    namespace backend {
      
#ifdef TARGET_IOS
      ALvoid  alBufferDataStatic(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq) {
        static	alBufferDataStaticProcPtr	proc = NULL;
        
        if (proc == NULL) {
          proc = (alBufferDataStaticProcPtr) alGetProcAddress((const ALCchar*) "alBufferDataStatic");
        }
        
        if (proc)
          proc(bid, format, data, size, freq);
        
        return;
      }
#endif

#ifdef TARGET_ANDROID
	    size_t android_fread(void *ptr, size_t size, size_t nmemb, void *datasource) {
	      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
	      FILE* handle = worker->handle;
      
	      long total = size * nmemb;
	      long pos = ftell(handle);
	      if (pos >= worker->ogg_maxPos) {
	        return 0;
	      }
      
	      if (pos + total >= worker->ogg_maxPos) {
	        total = worker->ogg_maxPos - pos;
        
	        return fread(ptr, total, 1, handle);
	      }
      
	      return fread(ptr, size, nmemb, handle);
	    }
    
	    int android_fseek(void *datasource, ogg_int64_t offset, int whence) {
	      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
	      FILE* handle = worker->handle;

	      if (whence == SEEK_END) {
	        return fseek(handle, worker->ogg_maxPos, SEEK_SET);
	      } else if (whence == SEEK_CUR) {
	        long pos = ftell(handle);
	        if (pos + offset > worker->ogg_maxPos) {
	          return 1;
	        }
        
	        return fseek(handle, offset, SEEK_CUR);
	      }
      
	      return fseek(handle, worker->ogg_start + offset, whence);
	    }
    
	    int android_fclose(void *datasource) {
	      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
	      FILE* handle = worker->handle;
      
	      return fclose(handle);
	    }
    
	    long android_ftell(void *datasource) {
	      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
	      FILE* handle = worker->handle;
      
	      return ftell(handle) - worker->ogg_start;
	    }
#endif

      
      MusicStreamWorker::MusicStreamWorker() {
        name = new char[80];
        name[0] = 0;
        
        loaded = false;
				
#ifdef TARGET_ANDROID
	      android_ogg_callbacks = new ov_callbacks();
	      android_ogg_callbacks->read_func = android_fread;
	      android_ogg_callbacks->seek_func = android_fseek;
	      android_ogg_callbacks->close_func = android_fclose;
	      android_ogg_callbacks->tell_func = android_ftell;
#endif
      }
      
      MusicStreamWorker::~MusicStreamWorker() {
        delete name;
        
        CloseOgg();
				
#ifdef TARGET_ANDROID
	      delete android_ogg_callbacks;
#endif
      }
      
      void MusicStreamWorker::Service() {
        while (alive) {
          while (!TryLock()) { if (!alive) break; }
          Wait();
          if (!alive) {
            Unlock();
            break;
          }
          
          if (!loaded) {
            Unlock();
            continue;
          }
          
          if (queue->Count() == 0) {
            Unlock();
            continue;
          }
          
          SwapQueues();
          
          toDo->Reset();
          while (toDo->Next()) {
            MusicBuffer* buff = toDo->GetCurrent()->GetValue();
            LoadPart(buff);
            
            while(!workDone->TryLock()) {}
            done->Append(buff);
            workDone->NotifyAll();
            workDone->Unlock();
          }
          
          toDo->RemoveAll();
          
          Unlock();
        }
        
        toDo->RemoveAll();
        queue->RemoveAll();
        done->RemoveAll();
        
        finished = true;
      }
      
      void MusicStreamWorker::Clear() {
        while(!TryLock()) {}
				
        while(!workDone->TryLock()) {}
        done->RemoveAll();
        workDone->NotifyAll();
        workDone->Unlock();
				
        queue->RemoveAll();
        toDo->RemoveAll();
        NotifyAll();
        Unlock();
      }
      
      void MusicStreamWorker::LoadPart(MusicBuffer* buff) {
        int size;
        int section;
        char *data = buff->data;
        
        size = 0;
        while (size < MUSIC_BUFFER_SIZE) {
          int result = ov_read(&oggStream, data + size, MUSIC_BUFFER_SIZE - size, &section);
          
          if (result > 0) {
            size += result;
          } else if (result == 0) {
            break;
          } else {
            LoadPart(buff);
            return;
          }
        }
        
        if (size == 0)  {
          ov_raw_seek(&oggStream, 0);
          
          LoadPart(buff);
          return;
        }
        
        alBufferDataStatic(buff->buffer, format, data, size, vorbisInfo->rate);
        int err = alGetError();
        if (err != AL_NO_ERROR) {
          std::cout << "THREAD " << buff->buffer << " = " << err << std::endl;
        }
      }
      
      bool MusicStreamWorker::PrepareMusic(const char *name) {
        while (!TryLock()) {}
        
        CloseOgg();
        
        loaded = false;
        
        Str n = name;
        n.GetChars(this->name, 80);
        
        LoadOgg();
        
        Unlock();
        
        return loaded;
      }
      
      void MusicStreamWorker::CloseOgg() {
        if (!loaded) return;
        
        ov_clear(&oggStream);
        ThePlatform->CloseFile(handle);
        
        loaded = false;
      }
      
      int MusicStreamWorker::GetFormat() const {
        return format;
      }
      
      void MusicStreamWorker::LoadOgg() {
        if (name[0] == 0) return;
        
        loaded = false;
        
        unsigned long size;
        handle = ThePlatform->OpenFile(name, size);
        if (handle <= 0) {
          return;
        }
        
#ifdef TARGET_ANDROID
	      ogg_start = ftell(handle);
	      ogg_maxPos = ftell(handle) + size;

	      int result = ov_open_callbacks(this, &oggStream, NULL, 0, *android_ogg_callbacks);
#elif TARGET_IOS
	      int result = ov_open(handle, &oggStream, NULL, (long)size);
#endif
        if (result < 0) {
          ThePlatform->CloseFile(handle);
          return;
        }
        
        vorbisInfo = ov_info(&oggStream, -1);
        if (vorbisInfo->channels == 1) {
          format = AL_FORMAT_MONO16;
        } else {
          format = AL_FORMAT_STEREO16;
        }
        
        loaded = true;
        
        Work();
      }
      
      OpenALSoundBackend::OpenALSoundBackend() {
        device = NULL;
        context = NULL;
        usedId = 0;
        nextId = 0;
        musicSource = 0;
        paused = true;
        worker = NULL;
        
        for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
          buffers[i].buffer = 0;
          buffers[i].data = NULL;
          buffers[i].loaded = false;
        }
      }
      
      OpenALSoundBackend::~OpenALSoundBackend() {
        Shutdown();
      }
      
      bool OpenALSoundBackend::Initialize() {
        if (device != NULL || context != NULL) {
          return true;
        }
        
        device = alcOpenDevice(NULL);
        
        if (device == NULL) {
          return false;
        }
        
        context = alcCreateContext(device, NULL);
        
        alcMakeContextCurrent(context);
        
        ALfloat listPos[] = { 0, 0, 0 };
        ALfloat listVel[] = { 0, 0, 0 };
        ALfloat listOri[] = { 0, 0, -1, 0, 1, 0 };
        
        alListenerfv(AL_POSITION, listPos);
        alListenerfv(AL_VELOCITY, listVel);
        alListenerfv(AL_ORIENTATION, listOri);
        
        alGenSources(1, &musicSource);
        alSourcef(musicSource, AL_PITCH, 1.0f);
        alSourcef(musicSource, AL_GAIN, 0.25f);
        alSourcei(musicSource, AL_LOOPING, 0);
        alSource3f(musicSource, AL_POSITION, 0, 0, 0);
        alSource3f(musicSource, AL_VELOCITY, 0, 0, 0);
        
        for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
          buffers[i].buffer = BufferCreate();
          buffers[i].data = new char[MUSIC_BUFFER_SIZE];
          buffers[i].loaded = false;
        }
        
        worker = new MusicStreamWorker();
        machete::thread::TheThreadMgr->Start(worker);

        return true;
      }
      
      void OpenALSoundBackend::Shutdown() {
        worker->Shutdown();
        delete worker;
        
        worker = NULL;
        
        for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
          if (buffers[i].data != NULL) {
            BufferDelete(buffers[i].buffer);
            delete buffers[i].data;
          }
          
          buffers[i].buffer = 0;
          buffers[i].data = NULL;
          buffers[i].loaded = false;
        }
        
        paused = true;

        if (context != NULL) {
          alcMakeContextCurrent(NULL);
          
          alcDestroyContext(context);
          
          context = NULL;
        }
        
        if (device != NULL) {
          alcCloseDevice(device);
          
          device = NULL;
        }
      }
      
      void OpenALSoundBackend::Pause() {
        alcSuspendContext(context);
        alcMakeContextCurrent(NULL);
#ifdef TARGET_ANDROID
	      alPauseThread();
#endif
      }
      
      void OpenALSoundBackend::Resume() {
#ifdef TARGET_ANDROID
	      alResumeThread();
#endif
        alcMakeContextCurrent(context);
        alcProcessContext(context);
      }
      
      unsigned int OpenALSoundBackend::SoundLoad(const char *name) {
        return ThePlatform->LoadAudio(name);
      }
      
      bool OpenALSoundBackend::BufferData(unsigned int buff, int format, char *data, int size, long rate) {
        alBufferDataStatic(buff, format, data, size, rate);
        int err = alGetError();
        return err == AL_NO_ERROR;
      }
      
      bool OpenALSoundBackend::StreamQueue(unsigned int source, unsigned int buff) {
        alSourceQueueBuffers(source, 1, &buff);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      bool OpenALSoundBackend::StreamUnqueue(unsigned int source, unsigned int * buffer) {
        alSourceUnqueueBuffers(source, 1, buffer);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      void OpenALSoundBackend::SourceDelete(unsigned int source) {
        alDeleteSources(1, &source);
      }
      
      void OpenALSoundBackend::BufferDelete(unsigned int buffer) {
        alDeleteBuffers(1, &buffer);
      }
      
      bool OpenALSoundBackend::SoundPlay(unsigned int source) {
        alSourcePlay(source);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      void OpenALSoundBackend::SoundStop(unsigned int source) {
        alSourceStop(source);
      }
      
      void OpenALSoundBackend::SoundPause(unsigned int source) {
        alSourcePause(source);
      }
      
      void OpenALSoundBackend::SoundRewind(unsigned int source) {
        alSourceRewind(source);
      }
      
      void OpenALSoundBackend::SoundSetVolume(unsigned int source, float vol) {
        alSourcef(source, AL_GAIN, vol);
      }
      
      bool OpenALSoundBackend::SoundIsPlaying(unsigned int source) {
        ALint val;
        
        alGetSourcei(source, AL_SOURCE_STATE, &val);
        return val == AL_PLAYING;
      }
      
      int OpenALSoundBackend::StreamQueuedBuffers(unsigned int source) {
        int queued;
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        return queued;
      }
      
      int OpenALSoundBackend::StreamProcessedBuffers(unsigned int source) {
        int processed;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
        return processed;
      }
      
      unsigned int OpenALSoundBackend::SoundCreate(unsigned int buffer) {
        unsigned int source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcef(source, AL_PITCH, 1.0f);
        alSourcef(source, AL_GAIN, 1.0f);
        alSourcei(source, AL_LOOPING, 0);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        
        return source;
      }
      
      unsigned int OpenALSoundBackend::BufferCreate() {
        unsigned int buffer;
        alGenBuffers(1, &buffer);
        
        return buffer;
      }
      
      void OpenALSoundBackend::SourceBind(unsigned int source, unsigned int buffer) {
        alSourcei(source, AL_BUFFER, buffer);
      }
      
      unsigned int OpenALSoundBackend::StreamCreate(const char* name) {
        Tree<Str, unsigned int> *node = inverse.Seek(name);
        if (node != NULL) {
          return node->GetValue();
        }
        
        nextId += 1;
        streams.Add(nextId, name);
        inverse.Add(name, nextId);
        
        return nextId;
      }
      
      void OpenALSoundBackend::StreamUnload() {
        if (usedId != 0) {
          worker->Clear();
          
          alSourceStop(musicSource);
          int total = StreamProcessedBuffers(musicSource);
          for (int i = 0; i < total; i++) {
            unsigned int unbuff = 0;
            if (!StreamUnqueue(musicSource, &unbuff)) {
              std::cout << "  ERR " << unbuff << std::endl;
            }
          }
          
          for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
            buffers[i].loaded = false;
          }
          
          worker->GetDoneResource()->Unlock();
          worker->Unlock();
          
          usedId = 0;
        }
      }
      
      void OpenALSoundBackend::StreamPreload() {
        for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
          MusicBuffer* buff = &buffers[i];
          worker->Push(buff);
        }
        
        worker->Work();
      }
      
      bool OpenALSoundBackend::StreamEnsure(unsigned int stream) {
        if (stream == usedId) {
          return true;
        }
        
        StreamUnload();
        
        Tree<unsigned int, Str> *node = streams.Seek(stream);
        if (node == NULL) {
          usedId = 0;
          return false;
        }
        
        usedId = stream;
        Str name = node->GetValue();
        char n[50];
        name.GetChars(n, 50);
        
        if (!worker->PrepareMusic(n)) {
          usedId = 0;
          return false;
        }
        
        StreamPreload();
        
        return true;
      }
      
      bool OpenALSoundBackend::StreamPlay(unsigned int source) {
        if (!StreamEnsure(source)) {
          return false;
        }
        
        paused = false;
        
        if (StreamQueuedBuffers(musicSource) > 0) {
          alSourcePlay(musicSource);
          
          return alGetError() == AL_NO_ERROR;
        }
        
        return true;
      }
      
      void OpenALSoundBackend::StreamStop(unsigned int source) {
        if (source != usedId) {
          return;
        }
        
        paused = true;
        StreamUnload();
      }
      
      void OpenALSoundBackend::StreamPause(unsigned int source) {
        if (source != usedId) {
          return;
        }
        
        paused = true;
        alSourcePause(musicSource);
      }
      
      void OpenALSoundBackend::StreamRewind(unsigned int source) {
        if (source != usedId) {
          return;
        }
        
        StreamStop(source);
        
        StreamPreload();
      }
      
      void OpenALSoundBackend::StreamSetVolume(float vol) {
        SoundSetVolume(musicSource, vol);
      }
      
      bool OpenALSoundBackend::StreamIsPlaying(unsigned int source) {
        if (source != usedId) {
          return false;
        }
        
        return SoundIsPlaying(musicSource);
      }
      
      void OpenALSoundBackend::Update(float time) {
        int processed;
        
        processed = StreamProcessedBuffers(musicSource);
        if (processed > 0) {
          while (processed-- > 0) {
            unsigned int unbuff;
            StreamUnqueue(musicSource, &unbuff);
            
            for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
              if (buffers[i].buffer == unbuff) {
                buffers[i].loaded = false;
                MusicBuffer* buff = &buffers[i];
                worker->Push(buff);
                break;
              }
            }
          }
        }
        
        while (!worker->GetDoneResource()->TryLock()) {}
        machete::data::Iterator<MusicBuffer*> *done = worker->GetDoneTasks();
        done->Reset();
        while (done->Next()) {
          StreamQueueOne(done->GetCurrent()->GetValue());
        }
        done->RemoveAll();
        worker->GetDoneResource()->Unlock();
                
        worker->Work();
      }
      
      void OpenALSoundBackend::StreamQueueOne(MusicBuffer *buff) {
        if (!StreamQueue(musicSource, buff->buffer)) {
          std::cout << "ENQUEUE " << std::endl;
        }
        buff->loaded = true;
        
        if (!paused && StreamQueuedBuffers(musicSource) > 0 && !StreamIsPlaying(usedId)) {
          alSourcePlay(musicSource);
        }
      }
      
      void OpenALSoundBackend::StreamDelete(unsigned int source) {
        Tree<unsigned int, Str> *node = streams.Seek(source);
        if (node == NULL) {
          return;
        }
        
        inverse.Delete(node->GetValue());
        streams.Delete(source);
      }
      
      SoundBackend* TheSoundBackend = NULL;

    }
  }
}
