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
    
    Sound::Sound(ALuint buffer, unsigned int cat) {
      ALfloat sourcePos[] = { 0, 0, 0 };
      ALfloat sourceVel[] = { 0, 0, 0 };
      
      alGenSources(1, &source);
      alSourcei(source, AL_BUFFER, buffer);
      alSourcef(source, AL_PITCH, 1.0f);
      alSourcef(source, AL_GAIN, 1.0f);
      alSourcei(source, AL_LOOPING, 0);
      alSourcefv(source, AL_POSITION, sourcePos);
      alSourcefv(source, AL_VELOCITY, sourceVel);
      
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
    
    unsigned int Sound::GetCategory() const {
      return category;
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
      
      Tree<Str, Sound*>* node = singletons.Seek(name);
      if (node == NULL) {
        Sound* snd = new Sound(buff, category);
        
        singletons.Add(name, snd);
        
        snd->Play();
        
        return snd;
      }
      
      Sound *snd = node->GetValue();
      
      snd->Rewind();
      snd->Play();
      
      return snd;
    }
    
    Sound* SoundManager::Play(const char *name, unsigned int category) {
      unsigned int buff = LoadBuffer(name);
      if (buff == 0) {
        return NULL;
      }
      
      if (sounds.Count() < MAX_SOUNDS) {
        if (available.Count() > 0) {
          Sound *snd = available.GetRoot()->GetValue();
          available.RemoveRoot();
          
          snd->Rebind(buff, category);
          
          sounds.Append(snd);
          
          snd->Play();
          return snd;
        }
        
        Sound* snd = new Sound(buff, category);
        
        sounds.Append(snd);
        
        snd->Play();
        return snd;
      } else {
        Sound* snd = sounds.GetRoot()->GetValue();
        sounds.RemoveRoot();
        
        snd->Rebind(buff, category);

        sounds.Append(snd);
        
        snd->Play();
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
    
    SoundManager* TheSoundMgr = NULL;
    
  }
}
