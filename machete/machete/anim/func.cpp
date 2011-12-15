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
    
    static float b = 0;
    static float c = 1;
    static float d = 1;
    
    float TweenLinear(float t) {
      return t;
    }
    
    float TweenEaseInQuad(float t) {
      return t * t;
    }
    
    float TweenEaseOutQuad(float t) {
      return - (t * (t - 2));
    }
    
    float TweenEaseInOutQuad(float t) {
      if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
      return -c/2 * (((--t)*(t-2)) - 1) + b;
    }
    
    float TweenEaseInCubic(float t) {
      return c*(t/=d)*t*t + b;
    }
    
    float TweenEaseOutCubic(float t) {
      return c*((t=t/d-1)*t*t + 1) + b;
    }
    
    float TweenEaseInOutCubic(float t) {
      if ((t/=d/2) < 1) return c/2*t*t*t + b;
      return c/2*((t-=2)*t*t + 2) + b;
    }
    
    float TweenEaseInQuart(float t) {
      return c*(t/=d)*t*t*t + b;
    }
    
    float TweenEaseOutQuart(float t) {
      return -c * ((t=t/d-1)*t*t*t - 1) + b;
    }
    
    float TweenEaseInOutQuart(float t) {
      if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
      return -c/2 * ((t-=2)*t*t*t - 2) + b;
    }
    
    float TweenEaseInQuint(float t) {
      return c*(t/=d)*t*t*t*t + b;
    }
    
    float TweenEaseOutQuint(float t) {
      return c*((t=t/d-1)*t*t*t*t + 1) + b;
    }
    
    float TweenEaseInOutQuint(float t) {
      if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
      return c/2*((t-=2)*t*t*t*t + 2) + b;
    }
    
    float TweenEaseInSine(float t) {
      return -c * cos(t/d * (machete::math::PiTwo)) + c + b;
    }
    
    float TweenEaseOutSine(float t) {
      return c * sin(t/d * (machete::math::PiTwo)) + b;
    }
    
    float TweenEaseInOutSine(float t) {
      return -c/2 * (cos(machete::math::Pi*t/d) - 1) + b;
    }
    
    float TweenEaseInExpo(float t) {
      return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;    
    }
    
    float TweenEaseOutExpo(float t) {
      return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
    }
    
    float TweenEaseInOutExpo(float t) {
      if (t==0) return b;
      if (t==d) return b+c;
      if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
      return c/2 * (-pow(2, -10 * --t) + 2) + b;
    }
    
    float TweenEaseInCirc(float t) {
      return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
    }
    
    float TweenEaseOutCirc(float t) {
      return c * sqrt(1 - (t=t/d-1)*t) + b;
    }
    
    float TweenEaseInOutCirc(float t) {
      if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
      return c/2 * (sqrt(1 - (t-=2)*t) + 1) + b;
    }
    
    float TweenEaseInElastic(float t) {
      if (t==0) return b;  if ((t/=d)==1) return b+c;
      float p=d*.3f;
      float a=c;
      float s=p/4;
      float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
      return -(postFix * sin((t*d-s)*(machete::math::TwoPi)/p )) + b;
    }
    
    float TweenEaseOutElastic(float t) {
      if (t==0) return b;  if ((t/=d)==1) return b+c;
      float p=d*.3f;
      float a=c;
      float s=p/4;
      return (a*pow(2,-10*t) * sin( (t*d-s)*(machete::math::TwoPi)/p ) + c + b);
    }
    
    float TweenEaseInOutElastic(float t) {
      if (t==0) return b;  if ((t/=d/2)==2) return b+c;
      float p=d*(.3f*1.5f);
      float a=c;
      float s=p/4;
      
      if (t < 1) {
        float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
        return -.5f*(postFix* sin( (t*d-s)*(machete::math::TwoPi)/p )) + b;
      }
      float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
      return postFix * sin( (t*d-s)*(machete::math::TwoPi)/p )*.5f + c + b;
    }
    
    float TweenEaseInBack(float t) {
      float s = 1.70158f;
      float postFix = t/=d;
      return c*(postFix)*t*((s+1)*t - s) + b;
    }
    
    float TweenEaseOutBack(float t) {
      float s = 1.70158f;
      return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
    }
    
    float TweenEaseInOutBack(float t) {
      float s = 1.70158f;
      if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
      float postFix = t-=2;
      return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
    }
    
    float TweenEaseInBounce(float t) {
      return c - TweenEaseOutBounce(d-t) + b;
    }
    
    float TweenEaseOutBounce(float t) {
      if ((t/=d) < (1/2.75f)) {
        return c*(7.5625f*t*t) + b;
      } else if (t < (2/2.75f)) {
        float postFix = t-=(1.5f/2.75f);
        return c*(7.5625f*(postFix)*t + .75f) + b;
      } else if (t < (2.5/2.75)) {
        float postFix = t-=(2.25f/2.75f);
        return c*(7.5625f*(postFix)*t + .9375f) + b;
      } else {
        float postFix = t-=(2.625f/2.75f);
        return c*(7.5625f*(postFix)*t + .984375f) + b;
      }
    }
    
    float TweenEaseInOutBounce(float t) {
      if (t < d/2) return TweenEaseInBounce(t*2) * .5f + b;
      else return TweenEaseOutBounce(t*2-d) * .5f + c*.5f + b;
    }
    
  }
}
