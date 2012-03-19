//
//  tween.cpp
//  machete
//
//  Created by Hilario Perez Corona on 3/18/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#include "tween.h"

namespace machete {
  namespace anim {
    
    Tween::Tween() {
      func = &TweenLinear;
      offset = 0;
      animation = 1.0f;
    }
    
    void Tween::SetTweenFunction(TweenFunc func) {
      this->func = func;
    }
    
    float Tween::GetValue(float time) const {
      return func(time * factor);
    }
    
    void Tween::SetTimes(float offset, float animation) {
      this->offset = offset;
      this->animation = animation;
      
      factor = 1.0f / animation;
    }
    
    Modifier::Modifier() {
      flags = 0;
    }
    
    void Modifier::Enable(unsigned long flag, machete::anim::ModifierType originRule, float originValue, machete::anim::ModifierType targetRule, float targetValue) {
      
      flags = flags | flag;
      
      if (flag & TargetPosX) {
        originRules[MODIFIER_SLOT_POS_X] = originRule;
        targetRules[MODIFIER_SLOT_POS_X] = targetRule;
        originModifiers[MODIFIER_SLOT_POS_X] = originValue;
        targetModifiers[MODIFIER_SLOT_POS_X] = targetValue;
      }

      if (flag & TargetPosY) {
        originRules[MODIFIER_SLOT_POS_Y] = originRule;
        targetRules[MODIFIER_SLOT_POS_Y] = targetRule;
        originModifiers[MODIFIER_SLOT_POS_Y] = originValue;
        targetModifiers[MODIFIER_SLOT_POS_Y] = targetValue;
      }

      if (flag & TargetRotation) {
        originRules[MODIFIER_SLOT_ROTATION] = originRule;
        targetRules[MODIFIER_SLOT_ROTATION] = targetRule;
        originModifiers[MODIFIER_SLOT_ROTATION] = originValue;
        targetModifiers[MODIFIER_SLOT_ROTATION] = targetValue;
      }

      if (flag & TargetColorR) {
        originRules[MODIFIER_SLOT_COLOR_R] = originRule;
        targetRules[MODIFIER_SLOT_COLOR_R] = targetRule;
        originModifiers[MODIFIER_SLOT_COLOR_R] = originValue;
        targetModifiers[MODIFIER_SLOT_COLOR_R] = targetValue;
      }

      if (flag & TargetColorG) {
        originRules[MODIFIER_SLOT_COLOR_G] = originRule;
        targetRules[MODIFIER_SLOT_COLOR_G] = targetRule;
        originModifiers[MODIFIER_SLOT_COLOR_G] = originValue;
        targetModifiers[MODIFIER_SLOT_COLOR_G] = targetValue;
      }

      if (flag & TargetColorB) {
        originRules[MODIFIER_SLOT_COLOR_B] = originRule;
        targetRules[MODIFIER_SLOT_COLOR_B] = targetRule;
        originModifiers[MODIFIER_SLOT_COLOR_B] = originValue;
        targetModifiers[MODIFIER_SLOT_COLOR_B] = targetValue;
      }

      if (flag & TargetColorA) {
        originRules[MODIFIER_SLOT_COLOR_A] = originRule;
        targetRules[MODIFIER_SLOT_COLOR_A] = targetRule;
        originModifiers[MODIFIER_SLOT_COLOR_A] = originValue;
        targetModifiers[MODIFIER_SLOT_COLOR_A] = targetValue;
      }

      if (flag & TargetScaleX) {
        originRules[MODIFIER_SLOT_SCALE_X] = originRule;
        targetRules[MODIFIER_SLOT_SCALE_X] = targetRule;
        originModifiers[MODIFIER_SLOT_SCALE_X] = originValue;
        targetModifiers[MODIFIER_SLOT_SCALE_X] = targetValue;
      }

      if (flag & TargetScaleY) {
        originRules[MODIFIER_SLOT_SCALE_Y] = originRule;
        targetRules[MODIFIER_SLOT_SCALE_Y] = targetRule;
        originModifiers[MODIFIER_SLOT_SCALE_Y] = originValue;
        targetModifiers[MODIFIER_SLOT_SCALE_Y] = targetValue;
      }
    }
    
    void Modifier::Disable(unsigned long flag) {
      flags = flags & ~flag;
    }
    
    void Modifier::Start(machete::draw::Element *elem) {
      if (flags & TargetPosX) {
        if (originRules[MODIFIER_SLOT_POS_X] == ModifierRelative) {
          originValues[MODIFIER_SLOT_POS_X] = elem->GetPosition().x + originModifiers[MODIFIER_SLOT_POS_X];
        } else {
          originValues[MODIFIER_SLOT_POS_X] = originModifiers[MODIFIER_SLOT_POS_X];
        }
        
        if (targetRules[MODIFIER_SLOT_POS_X] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_POS_X] = originValues[MODIFIER_SLOT_POS_X] + targetModifiers[MODIFIER_SLOT_POS_X];
        } else {
          targetValues[MODIFIER_SLOT_POS_X] = targetModifiers[MODIFIER_SLOT_POS_X];
        }
      }
      
      if (flags & TargetPosY) {
        if (originRules[MODIFIER_SLOT_POS_Y] == ModifierRelative) {
          originValues[MODIFIER_SLOT_POS_Y] = elem->GetPosition().y + originModifiers[MODIFIER_SLOT_POS_Y];
        } else {
          originValues[MODIFIER_SLOT_POS_Y] = originModifiers[MODIFIER_SLOT_POS_Y];
        }
        
        if (targetRules[MODIFIER_SLOT_POS_Y] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_POS_Y] = originValues[MODIFIER_SLOT_POS_Y] + targetModifiers[MODIFIER_SLOT_POS_Y];
        } else {
          targetValues[MODIFIER_SLOT_POS_Y] = targetModifiers[MODIFIER_SLOT_POS_Y];
        }
      }

      if (flags & TargetRotation) {
        if (originRules[MODIFIER_SLOT_ROTATION] == ModifierRelative) {
          originValues[MODIFIER_SLOT_ROTATION] = elem->GetRotation() + originModifiers[MODIFIER_SLOT_ROTATION];
        } else {
          originValues[MODIFIER_SLOT_ROTATION] = originModifiers[MODIFIER_SLOT_ROTATION];
        }
        
        if (targetRules[MODIFIER_SLOT_ROTATION] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_ROTATION] = originValues[MODIFIER_SLOT_ROTATION] + targetModifiers[MODIFIER_SLOT_ROTATION];
        } else {
          targetValues[MODIFIER_SLOT_ROTATION] = targetModifiers[MODIFIER_SLOT_ROTATION];
        }
      }
      
      if (flags & TargetColorR) {
        if (originRules[MODIFIER_SLOT_COLOR_R] == ModifierRelative) {
          originValues[MODIFIER_SLOT_COLOR_R] = elem->GetColor().x + originModifiers[MODIFIER_SLOT_COLOR_R];
        } else {
          originValues[MODIFIER_SLOT_COLOR_R] = originModifiers[MODIFIER_SLOT_COLOR_R];
        }
        
        if (targetRules[MODIFIER_SLOT_COLOR_R] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_COLOR_R] = originValues[MODIFIER_SLOT_COLOR_R] + targetModifiers[MODIFIER_SLOT_COLOR_R];
        } else {
          targetValues[MODIFIER_SLOT_COLOR_R] = targetModifiers[MODIFIER_SLOT_COLOR_R];
        }
      }
      
      if (flags & TargetColorG) {
        if (originRules[MODIFIER_SLOT_COLOR_G] == ModifierRelative) {
          originValues[MODIFIER_SLOT_COLOR_G] = elem->GetColor().y + originModifiers[MODIFIER_SLOT_COLOR_G];
        } else {
          originValues[MODIFIER_SLOT_COLOR_G] = originModifiers[MODIFIER_SLOT_COLOR_G];
        }
        
        if (targetRules[MODIFIER_SLOT_COLOR_G] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_COLOR_G] = originValues[MODIFIER_SLOT_COLOR_G] + targetModifiers[MODIFIER_SLOT_COLOR_G];
        } else {
          targetValues[MODIFIER_SLOT_COLOR_G] = targetModifiers[MODIFIER_SLOT_COLOR_G];
        }
      }

      if (flags & TargetColorB) {
        if (originRules[MODIFIER_SLOT_COLOR_B] == ModifierRelative) {
          originValues[MODIFIER_SLOT_COLOR_B] = elem->GetColor().z + originModifiers[MODIFIER_SLOT_COLOR_B];
        } else {
          originValues[MODIFIER_SLOT_COLOR_B] = originModifiers[MODIFIER_SLOT_COLOR_B];
        }
        
        if (targetRules[MODIFIER_SLOT_COLOR_B] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_COLOR_B] = originValues[MODIFIER_SLOT_COLOR_B] + targetModifiers[MODIFIER_SLOT_COLOR_B];
        } else {
          targetValues[MODIFIER_SLOT_COLOR_B] = targetModifiers[MODIFIER_SLOT_COLOR_B];
        }
      }

      if (flags & TargetColorA) {
        if (originRules[MODIFIER_SLOT_COLOR_A] == ModifierRelative) {
          originValues[MODIFIER_SLOT_COLOR_A] = elem->GetColor().w + originModifiers[MODIFIER_SLOT_COLOR_A];
        } else {
          originValues[MODIFIER_SLOT_COLOR_A] = originModifiers[MODIFIER_SLOT_COLOR_A];
        }
        
        if (targetRules[MODIFIER_SLOT_COLOR_A] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_COLOR_A] = originValues[MODIFIER_SLOT_COLOR_A] + targetModifiers[MODIFIER_SLOT_COLOR_A];
        } else {
          targetValues[MODIFIER_SLOT_COLOR_A] = targetModifiers[MODIFIER_SLOT_COLOR_A];
        }
      }
      
      if (flags & TargetScaleX) {
        if (originRules[MODIFIER_SLOT_SCALE_X] == ModifierRelative) {
          originValues[MODIFIER_SLOT_SCALE_X] = elem->GetScale().x + originModifiers[MODIFIER_SLOT_SCALE_X];
        } else {
          originValues[MODIFIER_SLOT_SCALE_X] = originModifiers[MODIFIER_SLOT_SCALE_X];
        }
        
        if (targetRules[MODIFIER_SLOT_SCALE_X] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_SCALE_X] = originValues[MODIFIER_SLOT_SCALE_X] + targetModifiers[MODIFIER_SLOT_SCALE_X];
        } else {
          targetValues[MODIFIER_SLOT_SCALE_X] = targetModifiers[MODIFIER_SLOT_SCALE_X];
        }
      }

      if (flags & TargetScaleY) {
        if (originRules[MODIFIER_SLOT_SCALE_Y] == ModifierRelative) {
          originValues[MODIFIER_SLOT_SCALE_Y] = elem->GetScale().y + originModifiers[MODIFIER_SLOT_SCALE_Y];
        } else {
          originValues[MODIFIER_SLOT_SCALE_Y] = originModifiers[MODIFIER_SLOT_SCALE_Y];
        }
        
        if (targetRules[MODIFIER_SLOT_SCALE_Y] == ModifierRelative) {
          targetValues[MODIFIER_SLOT_SCALE_Y] = originValues[MODIFIER_SLOT_SCALE_Y] + targetModifiers[MODIFIER_SLOT_SCALE_Y];
        } else {
          targetValues[MODIFIER_SLOT_SCALE_Y] = targetModifiers[MODIFIER_SLOT_SCALE_Y];
        }
      }
    }
    
    void Modifier::AffectValues(float interp) {
      for (int i = 0; i < MAX_MODIFIERS; i++) {
        values[i] = originValues[i] + ((targetValues[i] - originValues[i]) * interp);
      }
    }
    
    void Modifier::Apply(machete::draw::Element *elem) {
      unsigned long flag = 1;
      
      Vec2 pos = elem->GetPosition();
      Vec4 color = elem->GetColor();
      Vec2 scale = elem->GetScale();
      for (int i = 0; i < MAX_MODIFIERS; i++) {
        if (flag & flags) {
          switch (flag) {
            case TargetPosX:
              pos.x = values[i];
              break;
            case TargetPosY:
              pos.y = values[i];
              break;
            case TargetRotation:
              elem->SetRotation(values[i]);
              break;
            case TargetColorR:
              color.x = values[i];
              break;
            case TargetColorG:
              color.y = values[i];
              break;
            case TargetColorB:
              color.z = values[i];
              break;
            case TargetColorA:
              color.w = values[i];
              break;
            case TargetScaleX:
              scale.x = values[i];
              break;
            case TargetScaleY:
              scale.y = values[i];
              break;
            default:
              break;
          }
        }
        
        flag = flag << 1;
      }
      
      elem->SetPosition(pos);
      elem->SetColor(color);
      elem->SetScale(scale);
    }
    
    SingleTween::SingleTween(machete::draw::Element* element, TweenFunc func) : Tween(), Modifier() {
      this->element = element;
      offset = 0;
      time = 0;
      finished = false;
      started = false;
      SetTweenFunction(func);
    }
    
    SingleTween::~SingleTween() {
      
    }
    
    void SingleTween::Update(float time) {
      if (finished) return;
      
      this->time += time;
      
      if (this->time < offset) return;
      
      if (!started) {
        Start(element);
        started = true;
      }
      
      if (this->time > offset + animation) {
        this->time = offset + animation;
        finished = true;
      }
      
      AffectValues(GetValue(this->time - offset));
      Apply(element);
      
      if (finished) {
        element->Invalidate();
      }
    }
    
    bool SingleTween::IsFinished() const {
      return finished;
    }
    
    void SingleTween::Restart() {
      time = 0;
      started = false;
      finished = false;
    }
    
    TweenManager::TweenManager() {
      
    }
    
    TweenManager::~TweenManager() {
      
    }
    
    void TweenManager::Schedule(machete::anim::Tween *tween) {
      tweens.Append(tween);
    }
    
    void TweenManager::Update(float time) {
      tweens.Reset();
      while (tweens.Next()) {
        Tween* tween = tweens.GetCurrent()->GetValue();
        tween->Update(time);
      }
      
      tweens.Reset();
      tweens.Next();
      while (true) {
        if (tweens.GetCurrent() == NULL) return;
        
        if (tweens.GetCurrent()->GetValue()->IsFinished()) {
          tweens.RemoveCurrent(true);
        } else {
          if (!tweens.Next()) return;
        }
      }
    }
    
    bool TweenManager::IsAnimating() const {
      return tweens.Count() > 0;
    }
    
  }
}
