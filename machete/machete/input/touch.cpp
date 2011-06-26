//
//  touch.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/24/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "touch.h"

namespace machete {
  namespace input {
    
    TouchInput::TouchInput() {
      touchCount = 0;
      
      touches = new Touch*[10];
      for (int i = 0; i < 10; i++) {
        touches[i] = new Touch();
      }
      
      invalid = true;
    }
    
    TouchInput::~TouchInput() {
      for (int i = 0; i < 10; i++) {
        delete touches[i];
      }
      
      delete[] touches;
    }
    
    TouchInput *TheTouchInput = new TouchInput();
    
  }
}
