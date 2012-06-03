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
      source = 0;
      pause = true;
    }
    
    Music::~Music() {
      TheSoundBackend->StreamDelete(source);
    }
    
    bool Music::PrepareMusic(const char *name) {
      Stop();
      
      source = TheSoundBackend->StreamCreate(name);
      
      return source != 0;
    }
    
    void Music::Rewind() {
      TheSoundBackend->StreamRewind(source);
    }
    
    void Music::Play() {
      TheSoundBackend->StreamPlay(source);
    }
    
    void Music::Pause() {
      TheSoundBackend->StreamPause(source);
    }
    
    void Music::Resume() {
      TheSoundBackend->StreamPlay(source);
    }
    
    void Music::Stop() {
      TheSoundBackend->StreamStop(source);
    }
    
    void Music::SetVolume(float volume) {
      TheSoundBackend->StreamSetVolume(volume);
    }
    
    bool Music::IsPlaying() {
      return TheSoundBackend->StreamIsPlaying(source);
    }
    
    void Music::Update(float time) {
      
    }
    
    bool Music::IsLoaded() const {
      return source != 0;
    }
    
    Sound::Sound(unsigned int buffer, unsigned int cat) {
      source = TheSoundBackend->SoundCreate(buffer);
      TheSoundBackend->SoundSetVolume(source, 1.0f);
      
      category = cat;
      
      pause = true;
    }
    
    Sound::~Sound() {
      TheSoundBackend->SourceDelete(source);
      source = 0;
    }
    
    void Sound::Rebind(unsigned int buffer, unsigned int cat) {
      TheSoundBackend->SoundStop(source);
      TheSoundBackend->SourceBind(source, buffer);
      
      pause = true;
      category = cat;
    }
    
    void Sound::Rewind() {
      TheSoundBackend->SoundRewind(source);
    }
    
    void Sound::Play() {
      TheSoundBackend->SoundPlay(source);
      
      pause = false;
    }
    
    void Sound::Pause() {
      TheSoundBackend->SoundPause(source);
      
      pause = true;
    }
    
    void Sound::Resume() {
      TheSoundBackend->SoundPlay(source);
      
      pause = false;
    }
    
    void Sound::Stop() {
      TheSoundBackend->SoundStop(source);
      
      pause = true;
    }
    
    bool Sound::IsPlaying() {
      return TheSoundBackend->SoundIsPlaying(source);
    }
    
    unsigned int Sound::GetCategory() const {
      return category;
    }
    
    void Sound::SetVolume(float volume) {
      TheSoundBackend->SoundSetVolume(source, volume);
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
      backend::TheSoundBackend->Pause();
    }
    
    void SoundManager::Attach() {
      backend::TheSoundBackend->Resume();
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
        TheSoundBackend->BufferDelete(buff);
      }
      
      delete buffs;
      
      backend::TheSoundBackend->Shutdown();
    }
    
    bool SoundManager::Preload(const char *name) {
      Tree<Str, unsigned int>* node = buffers.Seek(name);
      if (node == NULL) {
        unsigned int buff = TheSoundBackend->SoundLoad(name);
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
      music = NULL;
      
      pausedMusicName = "";
    }
    
    MusicManager::~MusicManager() {
      delete music;
    }
    
    void MusicManager::Play(const char *file) {
      if (currentMusicName == file) {
        return;
      }
      
      if (music == NULL) {
        music = new Music();
      }
      
      currentMusicName = file;
      pausedMusicName = "";
      
      music->Stop();
      music->PrepareMusic(file);
      music->SetVolume(volume);
      music->Play();
    }
    
    void MusicManager::SetVolume(float volume) {
      if (this->volume == 0 && volume != 0 && music != NULL && music->IsPlaying() == false) {
        music->Play();
      }
      
      this->volume = volume;
      if (music != NULL) {
        music->SetVolume(volume);
      }
      
      if (volume == 0) {
        if (music != NULL) {
          music->Stop();
        }
      }
    }
    
    float MusicManager::GetVolume() {
      return volume;
    }
    
    void MusicManager::Update(float time) {
      if (music == NULL) return;
      
      if (volume == 0) return;
      
      music->Update(time);
      
      TheSoundBackend->Update(time);
    }
    
    void MusicManager::Stop() {
      if (music != NULL) {
        music->Stop();
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
    }
    
    void MusicManager::Resume() {
      if (pausedMusicName == "") {
        return;
      }
      
      char mname[50];
      pausedMusicName.GetChars(mname, 50);
      Play(mname);
    }
    
    SoundManager* TheSoundMgr = NULL;
    
    MusicManager* TheMusicMgr = NULL;
    
  }
}
