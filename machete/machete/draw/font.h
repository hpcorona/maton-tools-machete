//
//  font.h
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "meta.h"

namespace machete {
  namespace draw {
    
    class FontChar : public MetaSprite {
      void SetXOff(float x) { advance.x = x; }
      void SetYOff(float y) { advance.y = y; }
      inline float GetXOff() { return advance.x; }
      inline float GetYOff() { return advance.y; }
      void SetAdvance(const Vec2 & adv) { advance = adv; }
      const Vec2 & GetAdvance() { return advance; }
      
    protected:
      Vec2 advance;
    };
    
    class Text : public Element {
      
    };
    
    class Font {
      Font();
      ~Font();
      
      void Add(int c, FontChar*);
      FontChar* Get(int c);
      
      Text* Create(Str str);
      
    protected:
      Hash<int, FontChar*> chars;
    };
    
  }
}
