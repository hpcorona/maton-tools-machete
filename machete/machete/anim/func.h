//
//  func.h
//  machete
//
//  Created by Hilario Perez Corona on 7/25/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

namespace machete {
  
  //! Animation utilities.
  namespace anim {
    
    //! Tween animation function.
    typedef float (*TweenFunc)(float);

    // Tweening as seen on http://www.robertpenner.com/easing/penner_chapter7_tweening.pdf
    
    float TweenLinear(float time);
    
    float TweenEaseInQuad(float time);
    
    float TweenEaseOutQuad(float time);
    
    float TweenEaseInOutQuad(float time);
    
    float TweenEaseInCubic(float time);
    
    float TweenEaseOutCubic(float time);
    
    float TweenEaseInOutCubic(float time);
    
    float TweenEaseInQuart(float time);
    
    float TweenEaseOutQuart(float time);
    
    float TweenEaseInOutQuart(float time);
    
    float TweenEaseInQuint(float time);
    
    float TweenEaseOutQuint(float time);
    
    float TweenEaseInOutQuint(float time);
    
    float TweenEaseInSine(float time);

    float TweenEaseOutSine(float time);
    
    float TweenEaseInOutSine(float time);
    
    float TweenEaseInExpo(float time);
    
    float TweenEaseOutExpo(float time);
    
    float TweenEaseInOutExpo(float time);
    
    float TweenEaseInCirc(float time);
    
    float TweenEaseOutCirc(float time);
    
    float TweenEaseInOutCirc(float time);
    
    
    // Other tweening... http://code.google.com/p/cpptweener/source/browse/trunk/src/CppTweener.cpp
    
    float TweenEaseInElastic(float time);
    
    float TweenEaseOutElastic(float time);
    
    float TweenEaseInOutElastic(float time);
    
    float TweenEaseInBack(float time);
    
    float TweenEaseOutBack(float time);
    
    float TweenEaseInOutBack(float time);
    
    float TweenEaseInBounce(float time);
    
    float TweenEaseOutBounce(float time);
    
    float TweenEaseInOutBounce(float time);
    
  }
}
