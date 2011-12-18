//
//  sound.cpp
//  machete
//
//  Created by Hilario Perez Corona on 8/28/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "sound.h"
#include <iostream>


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
    
    MusicStreamWorker::MusicStreamWorker() {
      name = new char[80];
      name[0] = 0;
      
      loaded = false;
    }
    
    MusicStreamWorker::~MusicStreamWorker() {
      delete name;
      
      CloseOgg();
    }
    
    void MusicStreamWorker::Service() {
      
      while (alive) {
        while (!TryLock()) { if (!alive) return; }
        Wait();
        if (!alive) return;
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
        
        while (toDo->Count() > 0) {
          toDo->RemoveRoot();
        }
      }
      
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
      
      handle = ThePlatform->OpenFile(name);
      if (handle <= 0) {
        return;
      }
      
      int result;
      result = ov_open(handle, &oggStream, NULL, 0);
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
        alDeleteBuffers(1, &buffers[i].buffer);
        delete buffers[i].data;
      }

      alDeleteSources(1, &source);
    }
    
    bool Music::PrepareMusic(const char *name, bool preload) {
      Stop();
      
      soundLoaded = worker->PrepareMusic(name);

      format = worker->GetFormat();
      firstRun = true;
      
      if (!preload) {
        return soundLoaded;
      }
      
      Preload();
      
      return soundLoaded;
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
        std::cout << err << std::endl;
      }
      
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

      alSourcePlay(source);
      if (alGetError() != AL_NO_ERROR) {
        int err = alGetError();
        std::cout << err << std::endl;
      }
      
      pause = false;
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
      
      int processed;
      
      alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
      if (processed > 0) {
        while (processed-- > 0) {
          unsigned int unbuff;
          alSourceUnqueueBuffers(source, 1, &unbuff);
        
          for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
            if (buffers[i].buffer == unbuff) {
              MusicBuffer* buff = &buffers[i];
              worker->Push(buff);
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
        music->PrepareMusic(name, true);
        
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
      
      fading->PrepareMusic(file, true);
      
      if (flags == PlayInstant || fade <= 0) {
        current->Stop();
        fading->SetVolume(volume);
        fading->Play();
        
        maxTime = 0;
        time = 0;
      } else {
        maxTime = fade;
        time = fade;
        
        current->SetVolume(volume);
        fading->SetVolume(0);
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
    
    SoundManager* TheSoundMgr = NULL;
    
    MusicManager* TheMusicMgr = NULL;
    
  }
}
