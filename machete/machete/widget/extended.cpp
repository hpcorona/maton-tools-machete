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
      
      range.x = innPos.x;
      range.y = innPos.x + size.x - hBorder.x - hBorder.y;
      
      Container::Invalidate();
    }
    
    Trackbar::Trackbar(Widget *outer, Widget *inner, Actor *knob) : Progressbar(outer, inner) {
      this->knob = knob;
      knobCont = new Widget();
      knobCont->SetAllowDragX(true);
      knobCont->draw::Container::Add(knob);
      knobCont->Invalidate();
      knobCont->SetEventListener(this);
      
      knob->Invalidate();
      Rect2D kbound = knob->GetBounds();
      offset = kbound.pos;
      Vec2 offn = ZERO2 - offset;
      knob->SetPosition(offn);
      
      knobCont->SetSize(kbound.size);
      knobCont->Invalidate();
      
      outer->SetAllowDragX(true);
      outer->SetEventListener(this);
      
      changeListener = NULL;
      
      Add(knobCont);
    }
    
    Trackbar::~Trackbar() {
      delete knob;
      delete knobCont;
    }
    
    void Trackbar::Invalidate() {
      Vec2 pos = offset;
      pos.x += hBorder.x + porcentile * (size.x - hBorder.x - hBorder.y);
      pos.y += vBorder.x + (size.y - vBorder.x - vBorder.y) * 0.5f;
      
      knobCont->SetPosition(pos);
      knobCont->Invalidate();
      
      vpp = (max - min) / (range.y - range.x);
      
      Progressbar::Invalidate();
    }
    
    void Trackbar::WidgetStartDrag(machete::draw::Element *widget, Vec2 &position) {
      knob->Play("drag");
      
      if (widget == knobCont) {
        WidgetDragged(knobCont, ZERO2);
      } else {
        Rect2D size = inner->GetGlobalBounds();
        Vec2 local = position - size.pos;
        
        float nv = (local.x - hBorder.x) * vpp;
        
        SetValue(nv);
        Invalidate();
        
        if (changeListener != NULL) {
          changeListener->WidgetValueChanged(this);
        }
      }
    }
    
    void Trackbar::WidgetDragged(machete::draw::Element *widget, Vec2 &movement) {
      float inc = vpp * movement.x;
      
      Increment(inc);
      Invalidate();
      
      if (changeListener != NULL) {
        changeListener->WidgetValueChanged(this);
      }
    }
    
    void Trackbar::WidgetEndTouch(machete::draw::Element *widget) {
      Invalidate();
      
      if (porcentile == 0) {
        knob->Play("off");
      } else if (porcentile == 1) {
        knob->Play("on");
      } else {
        knob->Play("normal");
      }
    }
    
    void Trackbar::SetChangeListener(machete::widget::WidgetChangedAdapter *listener) {
      changeListener = listener;
    }
    
  }
}
