//
//  touch.h
//  machete
//
//  Created by Hilario Perez Corona on 6/24/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "../math/vector.h"

namespace machete {
  namespace input {
    
    enum TouchPhase {
      TouchStart,
      TouchMove,
      TouchStill,
      TouchEnd,
      TouchCancelled,
      TouchNone
    };
    
    struct Touch {
      Touch() : start(0,0), previous(0,0), current(0,0), offset(0,0) {
        reference = 0;
        finger = 0;
        phase = TouchNone;
        tapCount = 0;
      }
      
      void* reference;
      
      int finger;
      TouchPhase phase;
      machete::math::Vec2 start;
      machete::math::Vec2 previous;
      machete::math::Vec2 current;
      machete::math::Vec2 offset;
      
      int tapCount;
    };
    
    class TouchInput {
    public:
      TouchInput();
      ~TouchInput();
      
      inline void SetTouchCount(int count) { touchCount = count; }
      inline int GetTouchCount() const { return touchCount; }
      inline Touch* GetTouch(int idx) {
        return touches[idx];
      }
      
      void MarkAvailable() { invalid = false; }
      void Invalidate() { invalid = true; }
      inline bool IsAvailable() const { return !invalid; }
      
    protected:
      Touch **touches;
      int touchCount;
      bool invalid;
    };
    
    extern TouchInput *TheTouchInput;
    
  }
}
