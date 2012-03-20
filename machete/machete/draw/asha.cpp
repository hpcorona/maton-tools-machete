//
//  asha.cpp
//  machete
//
//  Created by Hilario Perez Corona on 3/18/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#include "asha.h"
#include "../data/mbd.h"

namespace machete {
  namespace draw {
    
    ScreenDefinition::ScreenDefinition(int width, int height) {
      this->width = width;
      this->height = height;
      area = width * height;
    }
    
    ScreenDefinition::~ScreenDefinition() {
      
    }
    
    int ScreenDefinition::GetWidth() const {
      return width;
    }
    
    int ScreenDefinition::GetHeight() const {
      return height;
    }
    
    int ScreenDefinition::GetArea() const {
      return area;
    }
    
    float ScreenDefinition::GetScaledArea(int width, int height) const {
      return (float)area * GetScaleForSize(width, height);
    }
    
    float ScreenDefinition::GetScaleForSize(int width, int height) const {
      float scw = (float)width / (float)this->width;
      float sch = (float)height / (float)this->height;
      
      return cmin(scw, sch);
      
    }
    
    const machete::data::Str ScreenDefinition::GetProp(machete::data::Str prop) {
      
      machete::data::Tree<machete::data::Str, machete::data::Str> *node = values.Seek(prop);
      if (node == NULL) {
        return "";
      }
      
      return node->GetValue();
    }
    
    void ScreenDefinition::SetProp(machete::data::Str &name, machete::data::Str &val) {
      values.Add(name, val);
    }
    
    ScreenAdapter::ScreenAdapter(Vec2 size, ScreenDefinition* screen, float scale, bool rotation, Vec2 & offset) {
      this->size = size;
      this->screen = screen;
      this->scale = scale;
      this->rotation = rotation;
      this->offset = offset;
      
      perfectMatch = scale == 1 && rotation == false && offset.x == 0 && offset.y == 0;
    }
    
    ScreenAdapter::~ScreenAdapter() {
      
    }
    
    void ScreenAdapter::AdaptRoot(machete::draw::Root *display, machete::draw::Element *element) {
      if (perfectMatch) {
        display->Add(element);
        return;
      }
      
      display->Add(element);
      display->SetRotation(rotation ? -machete::math::PiTwo : 0);
      Vec2 newOff = offset;
      
      if (rotation) {
        newOff.y -= size.y;
      }
      
      element->SetPosition(newOff);
      element->SetScale(scale, scale);
    }
    
    void ScreenAdapter::AdaptVector(Vec2 & touchPos) {
      if (perfectMatch) return;
      
      touchPos = (touchPos - offset) * (1.0f / scale);
      
      if (rotation) {
        float t = size.y - touchPos.x;
        touchPos.x = touchPos.y;
        touchPos.y = t;
      }
    }
    
    bool ScreenAdapter::IsPerfectMatch() const {
      return perfectMatch;
    }
    
    ScreenDefinition* ScreenAdapter::GetScreenDefinition() {
      return screen;
    }
    
    const machete::data::Str ScreenAdapter::GetProp(machete::data::Str prop) {
      return screen->GetProp(prop);
    }
    
    Asha::Asha(const char *filename) {
      Mbd *mbd = new Mbd(filename);
      
      int sCount = mbd->Count("/screens/screen");
      
      for (int sIdx = 1; sIdx <= sCount; sIdx++) {
        int width = mbd->IntValue("/screens/screen[%1]/@width", sIdx);
        int height = mbd->IntValue("/screens/screen[%1]/@height", sIdx);
        
        ScreenDefinition* screen = new ScreenDefinition(width, height);
        
        int pCount = mbd->Count("/screens/screen[%1]/prop", sIdx);
        for (int pIdx = 1; pIdx <= pCount; pIdx++) {
          Str name = mbd->Value("/screens/screen[%1]/prop[%2]/@name", sIdx, pIdx);
          Str value = mbd->Value("/screens/screen[%1]/prop[%2]/@value", sIdx, pIdx);
          
          screen->SetProp(name, value);
        }
        
        screens.Append(screen);
      }
      
      delete mbd;
    }
    
    Asha::~Asha() {
      
    }

    ScreenAdapter* Asha::Detect(int width, int height, bool allowRotate) {
      ScreenDefinition* upScale = SeekBestMatch(width, height, false);
      ScreenDefinition* downScale = SeekBestMatch(width, height, true);
      
      float area = width * height;
      
      if (downScale == upScale) {
        downScale = NULL;
      }
      
      float upArea = 0, downArea = 0, rotDownArea = 0, rotUpArea = 0;

      if (upScale != NULL) {
        upArea = cabs(area - upScale->GetScaledArea(width, height));
      }
      
      if (downScale != NULL) {
        downArea = cabs(area - downScale->GetScaledArea(width, height));
      }
      
      if (allowRotate) {
        ScreenDefinition* rotUpScale = SeekBestMatch(height, width, false);
        ScreenDefinition* rotDownScale = SeekBestMatch(height, width, true);
        
        if (rotDownScale == rotUpScale) {
          rotDownScale = NULL;
        }
        
        if (rotUpScale != NULL) {
          rotUpArea = cabs(area - rotUpScale->GetScaledArea(height, width));
        }
        
        if (rotDownScale != NULL) {
          rotDownArea = cabs(area - rotDownScale->GetScaledArea(height, width));
        }
        
        if (upScale != NULL && rotUpScale != NULL) {
          if (upArea < rotUpArea) {
            return MakeAdapter(upScale, false, width, height);
          } else {
            return MakeAdapter(rotUpScale, true, width, height);
          }
        } else if (rotUpScale != NULL) {
          return MakeAdapter(rotUpScale, true, width, height);
        } else if (upScale != NULL && rotDownScale != NULL) {
          return MakeAdapter(upScale, false, width, height);
        } else if (downScale != NULL && rotDownScale != NULL) {
          if (downArea < rotDownArea) {
            return MakeAdapter(downScale, false, width, height);
          } else {
            return MakeAdapter(rotDownScale, true, width, height);
          }
        }
      }
      
      if (upScale != NULL) {
        return MakeAdapter(upScale, false, width, height);
      }
      
      return MakeAdapter(downScale, false, width, height);
    }
    
    ScreenAdapter* Asha::MakeAdapter(machete::draw::ScreenDefinition *screen, bool rotate, int width, int height) {
      
      Vec2 size(width, height);
      
      float scale = screen->GetScaleForSize(width, height);
      
      Vec2 sizeNewScr((float)screen->GetWidth() * scale, (float)screen->GetHeight() * scale);
      
      if (rotate) {
        float t = size.x;
        size.x = size.y;
        size.y = t;
        
        t = sizeNewScr.x;
        sizeNewScr.x = sizeNewScr.y;
        sizeNewScr.y = t;
      }
      
      Vec2 diff = size - sizeNewScr;
      Vec2 offset = diff * 0.5f;
      
      ScreenAdapter* adapter = new ScreenAdapter(size, screen, scale, rotate, offset);
      
      return adapter;
    }
    
    ScreenDefinition* Asha::SeekBestMatch(int width, int height, bool useBigger) {
      float area = width * height;
      
      float minDiffArea = 0;
      ScreenDefinition *best = NULL;
      
      screens.Reset();
      while (screens.Next()) {
        ScreenDefinition* scrdf = screens.GetCurrent()->GetValue();
        
        if (!useBigger) {
          if (scrdf->GetWidth() > width || scrdf->GetHeight() > height) continue;
        }
        
        float a = scrdf->GetScaledArea(width, height);
        
        float da = cabs(area - a);
        
        if (best == NULL || minDiffArea > da) {
          best = scrdf;
        }
      }
      
      return best;
    }
    
    AshaManager::AshaManager() {
      adapter = NULL;
    }
    
    AshaManager::~AshaManager() {
      
    }
    
    void AshaManager::SetScreenAdapter(machete::draw::ScreenAdapter *adapter) {
      this->adapter = adapter;
      if (this->adapter != NULL && this->adapter->IsPerfectMatch() == true) {
        this->adapter = NULL;
      }
    }
    
    void AshaManager::AdaptPosition(Vec2 &pos) {
      if (adapter == NULL) return;
      
      this->adapter->AdaptVector(pos);
    }
    
    AshaManager* TheAshaManager = NULL;
    
  }
}
