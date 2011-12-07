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
      alive = false;
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
        if (bounds.Contains(touch->start) && touch->phase == TouchMove) {
          math::Vec2 startOff = touch->current - touch->start;
          
          bool isVertical = true;
          if (fabs(startOff.x) > fabs(startOff.y)) {
            isVertical = false;
          }
          if ((!isVertical && listener->TouchAcceptDragX()) || (isVertical && listener->TouchAcceptDragY())) {
            
            if (listener->TouchAcceptDragX() == false) {
              startOff.x = 0;
            }
            
            if (listener->TouchAcceptDragY() == false) {
              startOff.y = 0;
            }
            
            listener->TouchStartDrag(touch->start);
            listener->TouchDrag(startOff);
            alive = false;

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
          if (listener->TouchAcceptDragX() == false) {
            touch->offset.x = 0;
          }
          
          if (listener->TouchAcceptDragY() == false) {
            touch->offset.y = 0;
          }

          listener->TouchDrag(touch->offset);
          
          time = 0;
          alive = true;
          
          tracking = false;
        }
        
        listener->TouchEnded();
        
        this->touch = NULL;
        return true;
      }
      
      if (inside && touch->phase == TouchStart) {
        if (listener->TouchAcceptTap() && listener->TouchTapIntent() == true) {
          tracking = true;
          withTap = true;
          alive = false;
        } else {
          if (withTap) {
            listener->TouchTapCancelled();
          }
          
          withTap = false;
          
          if (listener->TouchAcceptDragX() || listener->TouchAcceptDragY()) {
            tracking = true;
            alive = false;
            
            listener->TouchStartDrag(touch->start);
          } else {
            tracking = false;
            alive = false;
            
            this->touch = NULL;
            return false;
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

          if (startOff.LengthSquared() > 400) {
            withTap = false;
            tracking = false;
            
            listener->TouchTapCancelled();
            
            return false;
          }
        }
        
        alive = false;
        inertia = touch->offset;
        listener->TouchDrag(touch->offset);
      }

      this->touch = NULL;
      return true;
    }
    
    void TouchProcessor::Update(float time) {
      if (alive) {
        this->time += time;
        if (inertia.LengthSquared() <= 1.0f) {
          alive = false;
          inertia.x = 0; inertia.y = 0;
        } else {
          inertia -= inertia * this->time;
        }
        
        listener->TouchInertia(inertia);
      }
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
    
    bool TouchProcessor::IsTracking() const {
      return tracking;
    }
    
    bool TouchProcessor::IsAlive() const {
      return alive;
    }
    
    void TouchProcessor::Stop() {
      alive = false;
    }
    
    TouchInput *TheTouchInput = new TouchInput();
    
  }
}
