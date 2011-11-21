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
    
    FILE* Platform::OpenFile(const char* name) {
      return platf->OpenFile(name);
    }
    
    void Platform::CloseFile(FILE* handle) {
      platf->CloseFile(handle);
    }
    
    char* Platform::WritableFile(const char* name) {
      return platf->WritableFile(name);
    }
    
    Platform* ThePlatform = NULL;
    
  }
}
