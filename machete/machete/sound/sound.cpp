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
    
    void SoundManager::PlaySingleton(const char* name) {
      Tree<Str, ALuint>* node = singletons.Seek(name);
      if (node == NULL) {
        ALuint source = NewSource(name);
        
        singletons.Add(name, source);
        
        alSourcePlay(source);
        return;
      }
      
      ALuint source = node->GetValue();
      
      alSourceRewind(source);
      alSourcePlay(source);
    }
    
    void SoundManager::Play(const char *name) {
      ALuint source = NewSource(name);
      alSourcePlay(source);
    }
    
    ALuint SoundManager::NewSource(const char *name) {
      Tree<Str, ALuint>* node = buffers.Seek(name);
      if (node == NULL) {
        if (Preload(name) == false) {
          return 0;
        }
        
        return NewSource(name);
      }
      
      ALuint buff = node->GetValue();
      ALuint source;
      
      ALfloat sourcePos[] = { 0, 0, 0 };
      ALfloat sourceVel[] = { 0, 0, 0 };
      
      alGenSources(1, &source);
      alSourcei(source, AL_BUFFER, buff);
      alSourcef(source, AL_PITCH, 1.0f);
      alSourcef(source, AL_GAIN, 1.0f);
      alSourcei(source, AL_LOOPING, 0);
      alSourcefv(source, AL_POSITION, sourcePos);
      alSourcefv(source, AL_VELOCITY, sourceVel);
      
      return source;
    }
    
    SoundManager* TheSoundMgr = NULL;
    
  }
}
