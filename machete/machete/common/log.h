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
    
    static void Log(const machete::data::Str & msg, ...) {
      machete::data::StrTpl tpl(msg);
      
      int count = tpl.GetNumParams();
      
      machete::data::StrParam p;
      
      va_list li;
      va_start(li, msg);
      machete::data::Str xs(20);
      int size = 0;
      
      for (int i = 0; i < count; i++) {
        float x = va_arg(li, float);
        xs = x;
        size += xs.Size();
        p.Append(xs);
      }
      
      va_end(li);
      
      int newSize = msg.Size();
      newSize += size;
      machete::data::Str base(newSize);
      
      tpl.Build(base, &p);

      char c[200];
      base.GetChars(c, 200);
      
      std::cout << c << std::endl;
    }
    
  }
}
