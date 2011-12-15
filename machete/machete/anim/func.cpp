//
//  func.cpp
//  machete
//
//  Created by Hilario Perez Corona on 7/25/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "func.h"
#include "../math/vector.h"

namespace machete {
  namespace anim {
    
    float TweenLinear(float time) {
      return time;
    }
    
    float TweenEaseInQuad(float time) {
      return time * time;
    }
    
    float TweenEaseOutQuad(float time) {
      return - (time * (time - 2));
    }
    
    float TweenEaseInOutQuad(float time) {
      time = time / 2.0f;
      
      if (time < 1) return time * time * 0.5f;
      
      time -= 1.0f;
      return (- (time * (time - 2)) - 1) * 0.5f;
    }
    
    float TweenEaseInCubic(float time) {
      return time * time * time;
    }
    
    float TweenEaseOutCubic(float time) {
      time = time - 1;
      
      return (time * time * time) + 1;
    }
    
    float TweenEaseInOutCubic(float time) {
      time = time * 0.5f;
      
      if (time < 1) {
        return (time * time * time) * 0.5f;
      } else {
        time = time - 2;
        
        return 0.5f * ((time * time * time) + 2);
      }
    }
    
    float TweenEaseInQuart(float time) {
      return time * time * time * time;
    }
    
    float TweenEaseOutQuart(float time) {
      time = time - 1;
      
      return - ((time * time * time * time) - 1);
    }
    
    float TweenEaseInOutQuart(float time) {
      time = time * 0.5f;
      
      if (time < 1) {
        return 0.5f * (time * time * time * time);
      } else {
        time = time * 0.5f;
        
        return -0.5f * ((time * time * time * time) - 2);
      }
    }
    
    float TweenEaseInQuint(float time) {
      return time * time * time * time * time;
    }
    
    float TweenEaseOutQuint(float time) {
      time = time - 1;
      
      return (time * time * time * time * time) + 1;
    }
    
    float TweenEaseInOutQuint(float time) {
      time = time * 0.5f;
      
      if (time < 1) {
        return 0.5f * (time * time * time * time * time);
      } else {
        time = time - 2.0f;
        
        return 0.5f * ((time * time * time * time * time) + 2);
      }
    }
    
    float TweenEaseInSine(float time) {
      return 1 - cos(time * machete::math::PiTwo);
    }
    
    float TweenEaseOutSine(float time) {
      return sin(time * machete::math::PiTwo);
    }
    
    float TweenEaseInOutSine(float time) {
      return 1 - cos(machete::math::Pi * time);
    }
    
    float TweenEaseInExpo(float time) {
      float v = 10 * (time - 1);
      return v * v;
    }
    
    float TweenEaseOutExpo(float time) {
      float v = -10 * time;
      return - (v * v) + 1;
    }
    
    float TweenEaseInOutExpo(float time) {
      time = time * 0.5f;
      if (time < 1) {
        float v = 10 * (time - 1);
        return 0.5f * v * v;
      } else {
        float v = -10 * (time - 1);
        return 0.5f * ((-v * v) + 2);
      }
    }
    
    float TweenEaseInCirc(float time) {
      return 1 - sqrt(1 - time * time);
    }
    
    float TweenEaseOutCirc(float time) {
      time = time - 1;
      
      return sqrt(1 - time * time);
    }
    
    float TweenEaseInOutCirc(float time) {
      time = time * 0.5f;
      
      if (time < 1) {
        return 0.5f * (1 - sqrt(1 - time * time));
      } else {
        time = time - 2;
        
        return 0.5f * (sqrt(1 - time * time) + 1);
      }
    }
    
    float TweenEaseInElastic(float time) {
      if (time == 0) return 0; if (time == 1) return 1;
      
      static float p = 0.3f;
      static float a = 1.0f;
      static float s = p / 4;
      
      float v = 10 * time;
      time -= 1;
      float postFix = a * v * v;
      return -(postFix * sin((time - s) * machete::math::TwoPi / p));
    }
    
    float TweenEaseOutElastic(float time) {
      if (time == 0) return 0; if (time == 1) return 1;
      
      static float p = 0.3f;
      static float a = 1.0f;
      static float s = p / 4;
      
      float v = -10 * time;
      
      return a * v * v * sin((time - s) * machete::math::TwoPi / p) + 1;
    }
    
    float TweenEaseInOutElastic(float time) {
      if (time == 0) return 0; if (time == 1) return 1;
      
      static float p = 0.3f * 1.5f;
      static float a = 1.0f;
      static float s = p / 4;
      
      time = time * 0.5f;
      
      if (time < 1) {
        float v = 10 * time;
        time = time - 1;
        float postFix = a * v * v;
        return -0.5f * (postFix * sin( (time - s) * machete::math::TwoPi / p));
      } else {
        float v = -10 * time;
        time = time - 1;
        float postFix = a * v * v;
        return postFix * sin( (time - s) * machete::math::TwoPi / p ) * 0.5f + 1;
      }
    }
    
    float TweenEaseInBack(float time) {
      static float s = 1.70158f;
      return time * time * ((s + 1) * time - s);
    }
    
    float TweenEaseOutBack(float time) {
      static float s = 1.70158f;
      
      time = time - 1;
      
      return (time * time * ((s + 1) * time + s)) + 1;
    }
    
    float TweenEaseInOutBack(float time) {
      float s = 1.70158f * 1.525f;

      time = time * 0.5f;
      
      if (time < 1) {
        return 0.5f * (time * time * ((s + 1) * time - s));
      } else {
        time -= 2;
        return 0.5f * time * time * ((s + 1) * time + s) + 2;
      }
    }
    
    float TweenEaseInBounce(float time) {
      return 1 - TweenEaseOutBounce(1 - time);
    }
    
    float TweenEaseOutBounce(float time) {
      if (time < (1 / 2.75f)) {
        return 7.5625f * time * time;
      } else if (time < (2 / 2.75f)) {
        float postFix = time;
        time -= 1.5f / 2.75f;
        return 7.5625f * postFix * time + 0.75f;
      } else if (time < (2.5f / 2.75f)) {
        float postFix = time;
        time -= 2.25f / 2.75f;
        return 7.5625f * postFix * time * 0.9375f;
      } else {
        float postFix = time;
        time -= (2.625f / 2.75f);
        return 7.5625f * postFix * time + 0.984375f;
      }
    }
    
    float TweenEaseInOutBounce(float time) {
      if (time < 0.5f) return TweenEaseInBounce(time * 2) * 0.5f;
      return TweenEaseOutBounce(time * 2 - 1) * 0.5f + 0.5f;
    }
    
  }
}
