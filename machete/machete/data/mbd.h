//
//  mbd.h
//  machete
//
//  Created by Hilario Perez Corona on 6/2/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include "tree.h"
#include "str.h"
#include "../platform/platform.h"

namespace machete { 
  namespace data {
    
    class Mbd {
    public:
      Mbd(const char *file);
      ~Mbd();
      
      int Count(const Str & qry, ...);
      const Str Value(const Str & qry, ...);
      int IntValue(const Str & qry, ...);
      float FloatValue(const Str & qry, ...);
      
      void ParseFile(const char *d);
      unsigned int ReadString(const char* data, Str **v) const;
      
    private:
      Hash<Str, unsigned int> counts;
      Hash<Str, Str> dict;
    };
    
  } 
}
