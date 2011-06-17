//
//  platform.h
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "../math/vector.h"
#include "../common/log.h"

namespace machete {
  
  class IPlatform {
  public:
    virtual const char* GetResourcePath() const = 0;
    virtual void LoadImage(const char* filename, char **data, machete::math::IVec2 & size) = 0;
    virtual unsigned int LoadFile(const char* name, char** data) = 0;
  };

  namespace platform {
    
    class Platform {
    public:
      Platform(machete::IPlatform* p) { platf = p; };
      const char* GetResourcePath() const;
      void LoadImage(const char* filename, char **data, machete::math::IVec2 & size);
      unsigned int LoadFile(const char* name, char** data);
      
    private:
      machete::IPlatform* platf;
    };
    
    extern Platform* ThePlatform;

  }
}
