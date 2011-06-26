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
    
    Platform* ThePlatform = NULL;
    
  }
}
