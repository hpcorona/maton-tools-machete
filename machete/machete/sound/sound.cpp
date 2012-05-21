//
//  sound.cpp
//  machete
//
//  Created by Hilario Perez Corona on 8/28/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "sound.h"

namespace machete {
  namespace sound {
    
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
      if (pos >= worker->maxPos) {
        return 0;
      }
      
      if (pos + total >= worker->maxPos) {
        total = worker->maxPos - pos;
        
        return fread(ptr, total, 1, handle);
      }
      
      return fread(ptr, size, nmemb, handle);
    }
    
    int android_fseek(void *datasource, ogg_int64_t offset, int whence) {
      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
      FILE* handle = worker->handle;

      if (whence == SEEK_END) {
        return fseek(handle, worker->maxPos, SEEK_SET);
      } else if (whence == SEEK_CUR) {
        long pos = ftell(handle);
        if (pos + offset > worker->maxPos) {
          return 1;
        }
        
        return fseek(handle, offset, SEEK_CUR);
      }
      
      return fseek(handle, worker->start + offset, whence);
    }
    
    int android_fclose(void *datasource) {
      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
      FILE* handle = worker->handle;
      
      return fclose(handle);
    }
    
    long android_ftell(void *datasource) {
      MusicStreamWorker* worker = static_cast<MusicStreamWorker*>(datasource);
      FILE* handle = worker->handle;
      
      return ftell(handle) - worker->start;
    }
#endif
    
    MusicStreamWorker::MusicStreamWorker() {
      name = new char[80];
      name[0] = 0;
      
#ifdef TARGET_ANDROID
      android_ogg_callbacks = new ov_callbacks();
      android_ogg_callbacks->read_func = android_fread;
      android_ogg_callbacks->seek_func = android_fseek;
      android_ogg_callbacks->close_func = android_fclose;
      android_ogg_callbacks->tell_func = android_ftell;
#endif

      loaded = false;
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
        while (!TryLock()) { if (!alive) return; }
        Wait();
        if (!alive) {
          Unlock();
          break;
        }
        
        if (queue->Count() == 0) {
          Unlock();
          continue;
        }
        
        SwapQueues();
        Unlock();
        
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
      }
      
      toDo->RemoveAll();
      queue->RemoveAll();
      done->RemoveAll();
      
      finished = true;
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
        std::cout << "THREAD " << err << std::endl;
      }
    }
    
    bool MusicStreamWorker::PrepareMusic(const char *name) {
      while (!TryLock()) {}
      
      CloseOgg();

      loaded = false;
      
      machete::data::Str n = name;
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
    
    ALenum MusicStreamWorker::GetFormat() const {
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
      start = ftell(handle);
      maxPos = ftell(handle) + size;

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
    }

    
    Music::Music() {
      firstRun = true;
      soundLoaded = false;
      
      worker = new MusicStreamWorker();
      machete::thread::TheThreadMgr->Start(worker);
      
      CreateBuffers();
    }
    
    Music::~Music() {
      worker->Shutdown();
      delete worker;
      
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        alSourceUnqueueBuffers(source, 1, &(buffers[i].buffer));
        alDeleteBuffers(1, &buffers[i].buffer);
        delete buffers[i].data;
      }

      alDeleteSources(1, &source);
    }
    
    void Music::PauseWorker() {
      if (worker->IsAlive()) {
        worker->Shutdown();
      }
    }
    
    void Music::ResumeWorker() {
      if (worker->IsAlive()) return;
      
      machete::thread::TheThreadMgr->Start(worker);
    }
    
    bool Music::PrepareMusic(const char *name) {
      Stop();
      
      soundLoaded = worker->PrepareMusic(name);

      format = worker->GetFormat();
      firstRun = true;
      
      return soundLoaded;
    }
    
    void Music::Unload() {
      while (worker->TryLock()) {}
      while (worker->GetDoneResource()->TryLock()) {}
      
      worker->GetDoneTasks()->RemoveAll();
      
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        if (buffers[i].loaded) {
          unsigned int unbuff = buffers[i].buffer;
          alSourceUnqueueBuffers(source, 1, &unbuff);
          int err = alGetError();
          if (err != AL_NO_ERROR) {
            std::cout << "UNLOAD " << err << std::endl;
          }
          buffers[i].loaded = false;
        }
      }
      
      worker->GetDoneResource()->Unlock();
      worker->Unlock();
    }
    
    void Music::Preload() {
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        MusicBuffer* buff = &buffers[i];
        worker->Push(buff);
      }
      
      worker->Work();
    }
    
    void Music::Enqueue(MusicBuffer *buff) {
      alSourceQueueBuffers(source, 1, &(buff->buffer));
      int err = alGetError();
      if (err != AL_NO_ERROR) {
        std::cout << "ENQUEUE " << err << std::endl;
      }
      buff->loaded = true;
      
      if (pause == false && !IsPlaying()) {
        Play();
      }
    }
    
    void Music::Rewind() {
      if (!soundLoaded) return;
      
      alSourceStop(source);
      
      if (pause == false) {
        alSourcePlay(source);
      }
    }
    
    void Music::Play() {
      if (!soundLoaded) return;

      pause = false;

      int queued;
      alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
      
      if (queued > 0) {
        alSourcePlay(source);
        int err = alGetError();
        if (err != AL_NO_ERROR) {
          std::cout << "PLAY " << err << std::endl;
        }
      }
    }
    
    void Music::Pause() {
      if (!soundLoaded) return;
      
      alSourcePause(source);
      
      pause = true;
    }
    
    void Music::Resume() {
      if (!soundLoaded) return;
      
      alSourcePlay(source);
      
      pause = false;
    }
    
    void Music::Stop() {
      if (!soundLoaded) return;
      
      alSourceStop(source);
      
      pause = true;
      
      Rewind();
    }
    
    void Music::SetVolume(float volume) {
       alSourcef(source, AL_GAIN, volume);
    }
    
    bool Music::IsPlaying() {
      if (!soundLoaded) return false;
      
      ALint val;
      
      alGetSourcei(source, AL_SOURCE_STATE, &val);
      return val == AL_PLAYING;
    }
    
    void Music::Update(float time) {
      if (!soundLoaded) return;
      
      if (firstRun) {
        firstRun = false;
        worker->Work();
        return;
      }

      int processed;
      
      alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
      if (processed > 0) {
        while (processed-- > 0) {
          unsigned int unbuff;
          alSourceUnqueueBuffers(source, 1, &unbuff);
        
          for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
            if (buffers[i].buffer == unbuff) {
              buffers[i].loaded = false;
              MusicBuffer* buff = &buffers[i];
              worker->Push(buff);
              break;
            }
          }
        }
        
        worker->Work();
      }
      
      while (!worker->GetDoneResource()->TryLock()) {}
      machete::data::Iterator<MusicBuffer*> *done = worker->GetDoneTasks();
      done->Reset();
      while (done->Next()) {
        Enqueue(done->GetCurrent()->GetValue());
      }
      done->RemoveAll();
      worker->GetDoneResource()->Unlock();
    }
    
    bool Music::IsLoaded() const {
      return soundLoaded;
    }
    
    void Music::CreateBuffers() {
      alGenSources(1, &source);
      alSourcef(source, AL_PITCH, 1.0f);
      alSourcef(source, AL_GAIN, 0.25f);
      alSourcei(source, AL_LOOPING, 0);
      alSource3f(source, AL_POSITION, 0, 0, 0);
      alSource3f(source, AL_VELOCITY, 0, 0, 0);
      
      pause = true;
      
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        alGenBuffers(1, &buffers[i].buffer);
        buffers[i].data = new char[MUSIC_BUFFER_SIZE];
        buffers[i].loaded = false;
      }
    }
    
    Sound::Sound(ALuint buffer, unsigned int cat) {
      alGenSources(1, &source);
      alSourcei(source, AL_BUFFER, buffer);
      alSourcef(source, AL_PITCH, 1.0f);
      alSourcef(source, AL_GAIN, 1.0f);
      alSourcei(source, AL_LOOPING, 0);
      alSource3f(source, AL_POSITION, 0, 0, 0);
      alSource3f(source, AL_VELOCITY, 0, 0, 0);
      
      category = cat;
      
      pause = true;
    }
    
    Sound::~Sound() {
      alDeleteSources(1, &source);
      source = 0;
    }
    
    void Sound::Rebind(ALuint buffer, unsigned int cat) {
      alSourceStop(source);
      alSourcei(source, AL_BUFFER, buffer);
      
      pause = true;
      category = cat;
    }
    
    void Sound::Rewind() {
      alSourceRewind(source);
    }
    
    void Sound::Play() {
      alSourcePlay(source);
      
      pause = false;
    }
    
    void Sound::Pause() {
      alSourcePause(source);
      
      pause = true;
    }
    
    void Sound::Resume() {
      alSourcePlay(source);
      
      pause = false;
    }
    
    void Sound::Stop() {
      alSourceStop(source);
      
      pause = true;
    }
    
    bool Sound::IsPlaying() {
      ALint val;
      
      alGetSourcei(source, AL_SOURCE_STATE, &val);
      return val == AL_PLAYING;
    }
    
    unsigned int Sound::GetCategory() const {
      return category;
    }
    
    void Sound::SetVolume(float volume) {
      alSourcef(source, AL_GAIN, volume);
    }
    
    SoundManager::SoundManager() {
      device = NULL;
      context = NULL;
    }
    
    SoundManager::~SoundManager() {
      Unload();
    }
    
    bool SoundManager::Initialize() {
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
      
      return true;
    }
    
    void SoundManager::Detach() {
      alcSuspendContext(context);
      alcMakeContextCurrent(NULL);
#ifdef TARGET_ANDROID
      alPauseThread();
#endif
    }
    
    void SoundManager::Attach() {
#ifdef TARGET_ANDROID
      alResumeThread();
#endif
      alcMakeContextCurrent(context);
      alcProcessContext(context);
    }
    
    void SoundManager::Unload() {
      Iterator< Tree<Str,Sound* >* >* singl = singletons.Enumerate();
      singl->Reset();
      while (singl->Next()) {
        Sound *snd = singl->GetCurrent()->GetValue()->GetValue();
        snd->Stop();
        delete snd;
      }
      
      delete singl;
      
      Iterator<Tree<Str,ALuint>*>* buffs = buffers.Enumerate();
      buffs->Reset();
      while (buffs->Next()) {
        ALuint buff = buffs->GetCurrent()->GetValue()->GetValue();
        alDeleteBuffers(1, &buff);
      }
      
      delete buffs;
      
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
    
    bool SoundManager::Preload(const char *name) {
      Tree<Str, ALuint>* node = buffers.Seek(name);
      if (node == NULL) {
        ALuint buff = ThePlatform->LoadAudio(name);
        if (buff == 0) {
          return false;
        }
        
        buffers.Add(name, buff);
      }
      
      return true;
    }
    
    Sound* SoundManager::PlaySingleton(const char* name, unsigned int category) {
      unsigned int buff = LoadBuffer(name);
      if (buff == 0) {
        return NULL;
      }
      
      float volume = GetVolume(category);
      
      Tree<Str, Sound*>* node = singletons.Seek(name);
      if (node == NULL) {
        Sound* snd = new Sound(buff, category);
        
        singletons.Add(name, snd);
        
        snd->SetVolume(volume);
        
        if (volume > 0) {
          snd->Play();
        }
        
        return snd;
      }
      
      Sound *snd = node->GetValue();
      
      snd->Rewind();
      snd->SetVolume(volume);
      
      if (volume > 0) {
        snd->Play();
      }
      
      return snd;
    }
    
    Sound* SoundManager::Play(const char *name, unsigned int category) {
      unsigned int buff = LoadBuffer(name);
      if (buff == 0) {
        return NULL;
      }
      
      float volume = GetVolume(category);
      
      if (sounds.Count() < MAX_SOUNDS) {
        if (available.Count() > 0) {
          Sound *snd = available.GetRoot()->GetValue();
          available.RemoveRoot();
          
          snd->Rebind(buff, category);
          
          sounds.Append(snd);
          
          snd->SetVolume(volume);
          
          if (volume > 0) {
            snd->Play();
          }
          
          return snd;
        }
        
        Sound* snd = new Sound(buff, category);
        
        sounds.Append(snd);
        
        snd->SetVolume(volume);
        
        if (volume > 0) {
          snd->Play();
        }
        
        return snd;
      } else {
        Sound* snd = sounds.GetRoot()->GetValue();
        sounds.RemoveRoot();
        
        snd->Rebind(buff, category);

        sounds.Append(snd);
        
        snd->SetVolume(volume);
        
        if (volume > 0) {
          snd->Play();
        }
        
        return snd;
      }
    }
    
    unsigned int SoundManager::LoadBuffer(const char* name) {
      Tree<Str, ALuint>* node = buffers.Seek(name);
      if (node == NULL) {
        if (Preload(name) == false) {
          return 0;
        }
        
        return LoadBuffer(name);
      }
      
      return node->GetValue();
    }
    
    Music* SoundManager::LoadMusic(const char* name) {
      Tree<Str, Music*>* node = musics.Seek(name);
      if (node == NULL) {
        Music* music = new Music();
        music->PrepareMusic(name);
        
        musics.Add(name, music);
        return music;
      }
      
      return node->GetValue();
    }
    
    void SoundManager::SetVolume(float volume, unsigned int category) {
      Tree<unsigned int, float> *node = volumes.Seek(category);
      if (node == NULL) {
        volumes.Add(category, volume);
      } else {
        node->SetValue(volume);
      }
      
      sounds.Reset();
      while (sounds.Next()) {
        Sound* snd = sounds.GetCurrent()->GetValue();
        
        if (snd->GetCategory() != category) {
          continue;
        }
        
        snd->SetVolume(volume);
      }
    }
    
    float SoundManager::GetVolume(unsigned int category) {
      Tree<unsigned int, float> *node = volumes.Seek(category);
      if (node == NULL) {
        return 0;
      } else {
        return node->GetValue();
      }
    }

    
    MusicManager::MusicManager() {
      volume = 0.4f;
      current = NULL;
      fading = NULL;
      time = 0;
      
      buff1 = NULL;
      buff2 = NULL;
      
      pausedMusicName = "";
    }
    
    MusicManager::~MusicManager() {
      delete buff1;
      delete buff2;
    }
    
    void MusicManager::Play(const char *file, unsigned int flags, float fade) {
      if (currentMusicName == file) {
        return;
      }
      
      if (buff1 == NULL || buff2 == NULL) {
        buff1 = new Music();
        buff2 = new Music();
        
        current = buff1;
        fading = buff2;
      }
      
      currentMusicName = file;
      pausedMusicName = "";
      
      fading->Stop();
      fading->Unload();
      fading->PrepareMusic(file);
      
      if (flags == PlayInstant || fade <= 0) {
        current->Stop();
        current->Unload();
        fading->SetVolume(volume);
        fading->Preload();
        fading->Play();
        
        maxTime = 0;
        time = 0;
      } else {
        maxTime = fade;
        time = fade;
        
        current->SetVolume(volume);
        fading->SetVolume(0);
        fading->Preload();
        fading->Play();
      }
      
      Music* mid = current;
      current = fading;
      fading = mid;
    }
    
    void MusicManager::SetVolume(float volume) {
      if (this->volume == 0 && volume != 0 && current != NULL && current->IsPlaying() == false) {
        current->Play();
      }
      
      this->volume = volume;
      
      if (time == 0 && current != NULL) {
        current->SetVolume(volume);
        
        if (volume == 0) {
          current->Stop();
        }
      }
    }
    
    float MusicManager::GetVolume() {
      return volume;
    }
    
    void MusicManager::Update(float time) {
      if (buff1 == NULL || buff2 == NULL) return;
      
      if (this->time > 0) {
        this->time -= time;
        if (this->time <= 0) {
          this->time = 0;
          fading->Stop();
          fading->Unload();
        }
        
        float factor = this->time / maxTime;
        
        current->SetVolume(volume * (1 - factor));
        fading->SetVolume(volume * factor);
      }
      
      if (volume == 0) return;
      
      current->Update(time);
      
      if (fading->IsPlaying()) {
        fading->Update(time);
      }
    }
    
    void MusicManager::Stop() {
      if (fading != NULL) {
        fading->Stop();
        fading->Unload();
      }
      
      if (current != NULL) {
        current->Stop();
        current->Unload();
      }
      
      currentMusicName = "";
      pausedMusicName = "";
    }
    
    void MusicManager::Pause() {
      if (currentMusicName == "") {
        pausedMusicName = "";
        Stop();
        return;
      }
      
      Str pmusic = currentMusicName;
      Stop();
      
      pausedMusicName = pmusic;
      
      if (fading != NULL) {
        fading->PauseWorker();
      }
      
      if (current != NULL) {
        current->PauseWorker();
      }
    }
    
    void MusicManager::Resume(unsigned int flags, float time) {
      if (pausedMusicName == "") {
        return;
      }
      
      if (fading != NULL) {
        fading->ResumeWorker();
      }
      
      if (current != NULL) {
        current->ResumeWorker();
      }
      
      char mname[50];
      pausedMusicName.GetChars(mname, 50);
      Play(mname, flags, time);
    }
    
    SoundManager* TheSoundMgr = NULL;
    
    MusicManager* TheMusicMgr = NULL;
    
  }
}
