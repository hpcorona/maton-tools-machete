//
//  extended.cpp
//  machete
//
//  Created by Hilario Perez Corona on 12/6/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "extended.h"

namespace machete {
  namespace widget {
    
    Progressbar::Progressbar(Widget *outer, Widget *inner) {
      allowTap = false;
      allowDragX = false;
      allowDragY = false;
      
      this->outer = outer;
      this->inner = inner;
      
      min = 0;
      max = 100;
      
      value = 50;
      porcentile = 50;
      
      Add(outer);
      Add(inner);
    }
    
    Progressbar::~Progressbar() {
      delete outer;
      delete inner;
    }
    
    void Progressbar::SetBorders(Vec2 &vSep, Vec2 &hSep) {
      vBorder = vSep;
      hBorder = hSep;
    }
    
    void Progressbar::SetRange(float min, float max) {
      this->min = min;
      this->max = max;
    }
    
    float Progressbar::GetMin() const {
      return min;
    }
    
    float Progressbar::GetMax() const {
      return max;
    }
    
    void Progressbar::SetValue(float val) {
      value = val;
      
      if (value > max) {
        value = max;
      } else if (value < min) {
        value = min;
      }
      
      if (max == min) {
        porcentile = 0;
      } else {
        porcentile = (value - min) / (max - min);
      }
    }
    
    float Progressbar::GetValue() const {
      return value;
    }
    
    float Progressbar::GetPorcentile() const {
      return porcentile;
    }
    
    void Progressbar::SetPorcentile(float porc) {
      SetValue(min + ((max - min) * porc));
    }
    
    void Progressbar::Increment(float diff) {
      SetValue(value + diff);
    }
    
    void Progressbar::SetSize(Vec2 &size) {
      this->size = size;
    }
    
    void Progressbar::SetSize(float w, float h) {
      size.x = w; size.y = h;
    }
    
    void Progressbar::Invalidate() {
      outer->SetPosition(ZERO2);
      outer->SetSize(size);
      
      Vec2 innSize;
      
      innSize.x = size.x - hBorder.x - hBorder.y;
      innSize.y = size.y - vBorder.x - vBorder.y;
      
      //FIXME: Multiply by Y if you want an horizontal progressbar.
      //FIXME: Maybe right-to-left progressbars? OH NO!!
      innSize.x = innSize.x * porcentile;
      
      Vec2 innPos(hBorder.x, vBorder.x);
      
      inner->SetPosition(innPos);
      inner->SetSize(innSize);
      
      TouchContainer::Invalidate();
    }
    
  }
}
