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
      touch = NULL;
    }
    
    bool TouchProcessor::Gather(Touch *touch, const machete::math::Rect2D & bounds) {
      
      if (listener == NULL) return false;
      
      this->touch = touch;
      
      bool inside = bounds.Contains(touch->current);
      
      if (!inside && !tracking) {
        this->touch = NULL;
        return false;
      }
      
      if (inside && !tracking) {
        if (touch->phase == TouchMove) {
          if (listener->TouchAcceptDrag()) {
            math::Vec2 startOff = touch->current - touch->start;
            listener->TouchDrag(startOff);

            tracking = true;
            withTap = false;
            
            this->touch = NULL;
            return true;
          } else {
            return false;
          }
        } else if (touch->phase != TouchStart) {
          this->touch = NULL;
          return false;
        }
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
        
        listener->TouchEnded();
        
        this->touch = NULL;
        return true;
      }
      
      if (inside && touch->phase == TouchStart) {
        if (listener->TouchTapIntent() == true) {
          tracking = true;
          withTap = true;
        } else {
          listener->TouchTapCancelled();
          withTap = false;
          
          if (listener->TouchAcceptDrag()) {
            tracking = true;
          }
        }
      } else if (tracking && touch->phase == TouchMove) {
        
        if (withTap) {
          math::Vec2 startOff = touch->current - touch->start;
          
          if (startOff.x < 0) {
            startOff.x = -startOff.x;
          }
          
          if (startOff.y < 0) {
            startOff.y = -startOff.y;
          }

          if (startOff.x > 10 || startOff.y > 10) {
            withTap = false;
            tracking = false;
            
            listener->TouchTapCancelled();
            
            return false;
          }
        }
        
        listener->TouchDrag(touch->offset);
      }

      this->touch = NULL;
      return true;
    }
    
    void TouchProcessor::Update(float time) {
      // TODO Inertia, acceleration, etc...
    }
    
    void TouchProcessor::Acquiere(TouchReceiver *owner) {
      if (touch != NULL) {
        touch->owner = owner;
      }
    }
    
    void TouchProcessor::Release() {
      if (touch != NULL) {
        touch->owner = NULL;
      }
    }
    
    TouchInput *TheTouchInput = new TouchInput();
    
  }
}
