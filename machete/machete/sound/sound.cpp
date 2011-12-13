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
    
    ALvoid  alBufferDataStatic(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq) {
      static	alBufferDataStaticProcPtr	proc = NULL;
      
      if (proc == NULL) {
        proc = (alBufferDataStaticProcPtr) alGetProcAddress((const ALCchar*) "alBufferDataStatic");
      }
      
      if (proc)
        proc(bid, format, data, size, freq);
      
      return;
    }
    
    Music::Music() {
      audioData = NULL;
      firstRun = true;
      soundLoaded = false;
      
      this->name = new char[80];
      this->name[0] = 0;
      
      CreateBuffers();
    }
    
    Music::~Music() {
      delete name;
      
      CloseOgg();
      
      if (audioData != NULL) {
        for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
          delete audioData[i];
        }
        
        delete [] audioData;
      }

      alDeleteBuffers(MAX_MUSIC_BUFFERS, buffers);
      alDeleteSources(1, &source);
    }
    
    void Music::CloseOgg() {
      if (!soundLoaded) return;
      
      Stop();
      
      ov_clear(&oggStream);
      ThePlatform->CloseFile(handle);
      
      soundLoaded = false;
      firstRun = true;
    }
    
    bool Music::PrepareMusic(const char *name, bool preload) {
      CloseOgg();
      
      machete::data::Str n = name;
      n.GetChars(this->name, 80);

      firstRun = true;
      
      if (!preload) {
        return true;
      }
      
      LoadOgg();
      
      return soundLoaded;
    }
    
    bool Music::Enqueue(int count) {
      if (!soundLoaded) return false;
      
      bool oneLoaded = false;
      
      while (count-- > 0) {
        int size;
        int section;
        char *data = audioData[currBuffer];
        
        size = 0;
        while (size < MUSIC_BUFFER_SIZE) {
          int result = ov_read(&oggStream, data + size, MUSIC_BUFFER_SIZE - size, &section);
          
          if (result > 0) {
            size += result;
          } else if (result == 0) {
            break;
          } else {
            return oneLoaded;
          }
        }
        
        if (size == 0)  {
          int ttl = ov_streams(&oggStream);
          
          if (section >= ttl) {
            ov_raw_seek(&oggStream, 0);
            
            return Enqueue(count + 1);
          }

          return oneLoaded; 
        }
        
        unsigned int buff = buffers[currBuffer];
        
        alBufferDataStatic(buff, format, data, size, vorbisInfo->rate);
        int err = alGetError();
        if (err != AL_NO_ERROR) {
          std::cout << err << std::endl;
        }
          
        alSourceQueueBuffers(source, 1, &buff);
        err = alGetError();
        if (err != AL_NO_ERROR) {
          std::cout << err << std::endl;
        }
        
        currBuffer = (currBuffer + 1) % MAX_MUSIC_BUFFERS;
        
        oneLoaded = true;
        
        int ttl = ov_streams(&oggStream);
        
        if (section >= ttl) {
          ov_raw_seek(&oggStream, 0);
        }
      }
      
      int queued;
      alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
      
      return true;
    }
    
    void Music::Rewind() {
      LoadOgg();
      if (!soundLoaded) return;
      
      alSourceStop(source);
      
      if (pause == false) {
        alSourcePlay(source);
      }
    }
    
    void Music::Play() {
      LoadOgg();
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
      LoadOgg();
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
        alSourceUnqueueBuffers(source, processed, unbuff);
      
        Enqueue(processed);
      }
    }
    
    bool Music::IsLoaded() const {
      return soundLoaded;
    }
    
    void Music::LoadOgg() {
      if (!firstRun) return;
      if (name[0] == 0) return;
      
      firstRun = false;
      soundLoaded = false;
      
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
      
      soundLoaded = true;
      
      pause = true;
      
      Enqueue(MAX_MUSIC_BUFFERS);
    }
    
    void Music::CreateBuffers() {
      alGenBuffers(MAX_MUSIC_BUFFERS, buffers);
      currBuffer = 0;
      
      alGenSources(1, &source);
      alSourcef(source, AL_PITCH, 1.0f);
      alSourcef(source, AL_GAIN, 0.25f);
      alSourcei(source, AL_LOOPING, 0);
      alSource3f(source, AL_POSITION, 0, 0, 0);
      alSource3f(source, AL_VELOCITY, 0, 0, 0);
      
      pause = true;
      
      audioData = new char*[MAX_MUSIC_BUFFERS];
      for (int i = 0; i < MAX_MUSIC_BUFFERS; i++) {
        audioData[i] = new char[MUSIC_BUFFER_SIZE];
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
