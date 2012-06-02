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
        TheSoundBackend->UnqueueBuffer(source, &(buffers[i].buffer));
        TheSoundBackend->DeleteBuffer(buffers[i].buffer);
        delete buffers[i].data;
      }

      TheSoundBackend->DeleteSource(source);
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
          if (TheSoundBackend->UnqueueBuffer(source, &unbuff) == false) {
            std::cout << "UNLOAD " << std::endl;
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
      if (!TheSoundBackend->QueueBuffer(source, buff->buffer)) {
        std::cout << "ENQUEUE " << std::endl;
      }
      buff->loaded = true;
      
      if (pause == false && !IsPlaying()) {
        Play();
      }
    }
    
    void Music::Rewind() {
      if (!soundLoaded) return;
      
      TheSoundBackend->Stop(source);
      
      if (pause == false) {
        TheSoundBackend->Play(source);
      }
    }
    
    void Music::Play() {
      if (!soundLoaded) return;

      pause = false;

      int queued;
      queued = TheSoundBackend->QueuedBuffers(source);
      
      if (queued > 0) {
        if (!TheSoundBackend->Play(source)) {
          std::cout << "PLAY " << std::endl;
        }
      }
    }
    
    void Music::Pause() {
      if (!soundLoaded) return;
      
      TheSoundBackend->Pause(source);
      
      pause = true;
    }
    
    void Music::Resume() {
      if (!soundLoaded) return;

      TheSoundBackend->Play(source);
      
      pause = false;
    }
    
    void Music::Stop() {
      if (!soundLoaded) return;
      
      TheSoundBackend->Stop(source);
      
      pause = true;
      
      Rewind();
    }
    
    void Music::SetVolume(float volume) {
      TheSoundBackend->SetVolume(source, volume);
    }
    
    bool Music::IsPlaying() {
      if (!soundLoaded) return false;
      
      return TheSoundBackend->IsPlaying(source);
    }
    
    void Music::Update(float time) {
      if (!soundLoaded) return;
      
      if (firstRun) {
        firstRun = false;
        worker->Work();
        return;
      }

      int processed;
      
      processed = TheSoundBackend->ProcessedBuffers(source);
      if (processed > 0) {
        while (processed-- > 0) {
          unsigned int unbuff;
          TheSoundBackend->UnqueueBuffer(source, &unbuff);
        
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
      source = TheSoundBackend->CreateSource(0.25f);
      
      pause = true;
      
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        buffers[i].buffer = TheSoundBackend->CreateBuffer();
        buffers[i].data = new char[MUSIC_BUFFER_SIZE];
        buffers[i].loaded = false;
      }
    }
    
    Sound::Sound(unsigned int buffer, unsigned int cat) {
      source = TheSoundBackend->CreateSource(1.0f);
      TheSoundBackend->BindBufferToSource(buffer, source);
      
      category = cat;
      
      pause = true;
    }
    
    Sound::~Sound() {
      TheSoundBackend->DeleteSource(source);
      source = 0;
    }
    
    void Sound::Rebind(unsigned int buffer, unsigned int cat) {
      TheSoundBackend->Stop(source);
      TheSoundBackend->BindBufferToSource(buffer, source);
      
      pause = true;
      category = cat;
    }
    
    void Sound::Rewind() {
      TheSoundBackend->Rewind(source);
    }
    
    void Sound::Play() {
      TheSoundBackend->Play(source);
      
      pause = false;
    }
    
    void Sound::Pause() {
      TheSoundBackend->Pause(source);
      
      pause = true;
    }
    
    void Sound::Resume() {
      TheSoundBackend->Play(source);
      
      pause = false;
    }
    
    void Sound::Stop() {
      TheSoundBackend->Stop(source);
      
      pause = true;
    }
    
    bool Sound::IsPlaying() {
      return TheSoundBackend->IsPlaying(source);
    }
    
    unsigned int Sound::GetCategory() const {
      return category;
    }
    
    void Sound::SetVolume(float volume) {
      TheSoundBackend->SetVolume(source, volume);
    }
    
    SoundManager::SoundManager() {
    }
    
    SoundManager::~SoundManager() {
      Unload();
    }
    
    bool SoundManager::Initialize() {
      return backend::TheSoundBackend->Initialize();
    }
    
    void SoundManager::Detach() {
      backend::TheSoundBackend->Detach();
    }
    
    void SoundManager::Attach() {
      backend::TheSoundBackend->Attach();
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
      
      Iterator<Tree<Str,unsigned int>*>* buffs = buffers.Enumerate();
      buffs->Reset();
      while (buffs->Next()) {
        unsigned int buff = buffs->GetCurrent()->GetValue()->GetValue();
        TheSoundBackend->DeleteBuffer(buff);
      }
      
      delete buffs;
      
      backend::TheSoundBackend->Shutdown();
    }
    
    bool SoundManager::Preload(const char *name) {
      Tree<Str, unsigned int>* node = buffers.Seek(name);
      if (node == NULL) {
        unsigned int buff = TheSoundBackend->LoadSound(name);
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
      Tree<Str, unsigned int>* node = buffers.Seek(name);
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
