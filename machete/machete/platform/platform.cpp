//
//  platform.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/17/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "platform.h"

namespace machete {
  namespace platform {
    
    const char* Platform::GetResourcePath() const {
      return platf->GetResourcePath();
    }
    
    void Platform::LoadImage(const char* name, void **data, machete::math::IVec2 & size) {
      platf->LoadImage(name, data, size);
    }
    
    void Platform::UnloadImage() {
      platf->UnloadImage();
    }
    
    unsigned int Platform::LoadFile(const char* name, char** data) {
      return platf->LoadFile(name, data);
    }
    
    unsigned int Platform::LoadAudio(const char *name) {
      return platf->LoadAudio(name);
    }
    
    unsigned int Platform::Random() {
      return platf->Random();
    }
    
    bool Platform::LoadMusicInfo(const char* name, unsigned int &maxPSize, unsigned int &pCount, int & auFormat, int & freq) {
      return platf->LoadMusicInfo(name, maxPSize, pCount, auFormat, freq);
    }
    
    bool Platform::LoadMusicBuffers(const char* name, unsigned int maxPacketSize, unsigned int offset, unsigned int count, unsigned int &packsLoaded, void *audioData, unsigned int &bytesLoaded) {
      return platf->LoadMusicBuffers(name, maxPacketSize, offset, count, packsLoaded, audioData, bytesLoaded);
    }
    
    Platform* ThePlatform = NULL;
    
  }
}
