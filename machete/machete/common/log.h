//
//  log.h
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "../data/str.h"
#include <iostream>

namespace machete {
  namespace common {
    
    static void Log(const machete::data::Str & msg) {
      int c = 0;
      
      while (c < msg.Size()) {
        int w = msg.CharSizeAt(c);
        
        wchar_t wc = msg.CharAt(c);
        
        std::cout << wc;
        
        c += w;
      }
      
      std::cout << std::endl;
    }
    
  }
}
