//
//  tween.h
//  machete
//
//  Created by Hilario Perez Corona on 3/18/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#pragma once

#include "func.h"
#include "../draw/meta.h"
#include "../data/lists.h"

namespace machete {
  namespace anim {
    
    //! Animation Targets
    enum AnimTarget {
      TargetPosX = 1,       //!< Animate X position
      TargetPosY = 2,       //!< Animate Y position
      TargetRotation = 4,   //!< Animate Rotation
      TargetColorR = 8,     //!< Animate Red Color
      TargetColorG = 16,    //!< Animate Green Color
      TargetColorB = 32,    //!< Animate Blue Color
      TargetColorA = 64,    //!< Animate Alpha Color
      TargetScaleX = 128,   //!< Animate X Scale
      TargetScaleY = 256    //!< Animate Y Scale
    };
    
#define MODIFIER_SLOT_POS_X 0
#define MODIFIER_SLOT_POS_Y 1
#define MODIFIER_SLOT_ROTATION 2
#define MODIFIER_SLOT_COLOR_R 3
#define MODIFIER_SLOT_COLOR_G 4
#define MODIFIER_SLOT_COLOR_B 5
#define MODIFIER_SLOT_COLOR_A 6
#define MODIFIER_SLOT_SCALE_X 7
#define MODIFIER_SLOT_SCALE_Y 8
#define MAX_MODIFIERS 9
    
    enum ModifierType {
      ModifierAbsolute = 1,
      ModifierRelative = 0
    };
    
    class Tween {
    public:
      
      Tween();
      
      virtual ~Tween() {};
      
      virtual void Update(float time) = 0;
      
      virtual bool IsFinished() const = 0;
      
      virtual void Restart() = 0;
      
      void SetTweenFunction(TweenFunc func);
      
      float GetValue(float time) const;
      
      void SetTimes(float offset, float animation);
      
    protected:
      
      TweenFunc func;
      
      float offset;
      
      float animation;
      
      float factor;
      
    };
    
    class Modifier {
    public:
      
      Modifier();

      void Enable(unsigned long flag, ModifierType originRule, float originValue, ModifierType targetRule, float targetValue);
      
      void Disable(unsigned long flag);
      
    protected:
      
      void Start(machete::draw::Element* elem);
      
      void AffectValues(float interp);
      
      void Apply(machete::draw::Element* elem);

      unsigned long flags;
      
      ModifierType originRules[MAX_MODIFIERS];
      
      ModifierType targetRules[MAX_MODIFIERS];
      
      float originModifiers[MAX_MODIFIERS];
      
      float targetModifiers[MAX_MODIFIERS];
      
      float originValues[MAX_MODIFIERS];
      
      float targetValues[MAX_MODIFIERS];
      
      float values[MAX_MODIFIERS];
      
    };
    
    class SingleTween : public Modifier, public Tween {
    public:
      
      SingleTween(machete::draw::Element* element, TweenFunc func);
      
      ~SingleTween();
      
      void Update(float time);
      
      bool IsFinished() const;
      
      void Restart();
      
    protected:
      
      machete::draw::Element* element;
      
      float time;
      
      bool finished;
      
      bool started;
      
    };
    
    class TweenManager {
    public:
      
      TweenManager();
      
      virtual ~TweenManager();
      
      void Schedule(Tween *tween);
      
      void Update(float time);
      
      bool IsAnimating() const;
      
    protected:
      
      machete::data::Iterator<Tween*> tweens;
      
    };
    
  }
}
