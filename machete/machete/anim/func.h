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
  //! As found in: 
  //! http://unknownerror.net/2011-05/javascript-tween-algorithm-and-the-slow-moving-effect-turn-20484
  namespace anim {
    
    //! Tween animation function.
    typedef float (*TweenFunc)(float, float, float);
    
    //! Ease Out effect.
    float TweenEaseOut(float time, float duration, float length);
    
  }
}
