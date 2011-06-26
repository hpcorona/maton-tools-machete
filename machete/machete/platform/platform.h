//
//  platform.h
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include "../math/vector.h"
#include "../common/log.h"

namespace machete {
  
  class IPlatform {
  public:
    virtual const char* GetResourcePath() const = 0;
    virtual void LoadImage(const char* filename, void **data, machete::math::IVec2 & size) = 0;
    virtual void UnloadImage() = 0;
    virtual unsigned int LoadFile(const char* name, char** data) = 0;
  };

  namespace platform {
    
    class Platform {
    public:
      Platform(machete::IPlatform* p) { platf = p; };
      const char* GetResourcePath() const;
      void LoadImage(const char* filename, void **data, machete::math::IVec2 & size);
      void UnloadImage();
      unsigned int LoadFile(const char* name, char** data);
      
    private:
      machete::IPlatform* platf;
    };
    
    extern Platform* ThePlatform;

  }
}
