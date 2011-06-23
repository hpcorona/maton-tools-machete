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
      char c[200];
      msg.GetChars(c, 200);
      
      std::cout << c << std::endl;
    }
    
  }
}
