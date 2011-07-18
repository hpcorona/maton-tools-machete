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
    
    TouchProcessor::TouchProcessor(TouchListener *listener) {
      this->listener = listener;
      withTap = false;
      tracking = false;
    }
    
    bool TouchProcessor::Gather(Touch *touch, const machete::math::Rect2D & bounds) {
      
      if (listener == NULL) return false;
      
      bool inside = bounds.Contains(touch->current);
      
      if (!inside && !tracking) {
        return false;
      }
      
      if (inside && !tracking && touch->phase != TouchStart) {
        return false;
      }
      
      if (tracking && touch->phase == TouchEnd) {
        if (withTap) {
          if (inside) {
            listener->TouchTapPerformed();
          } else {
            listener->TouchTapCancelled();
          }
        
          tracking = false;
          withTap = false;
        } else {
          // TODO Touch end without tap, it was dragging
          // prepare inertia?
          
          tracking = false;
          
        }
        return true;
      }
      
      if (inside && touch->phase == TouchStart) {
        tracking = true;
        withTap = true;
        
        listener->TouchTapIntent();
      } else if (inside && touch->phase == TouchMove) {
        
        if (withTap) {
          listener->TouchTapCancelled();
          withTap = false;
        }
        
        listener->TouchDrag(touch->offset);
      }

      return true;
    }
    
    void TouchProcessor::Update(float time) {
      // TODO Inertia, acceleration, etc...
    }
    
    TouchInput *TheTouchInput = new TouchInput();
    
  }
}
