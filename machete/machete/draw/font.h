//
//  font.h
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "meta.h"
#include "../data/mbd.h"

using namespace machete::data;
using namespace machete::math;

namespace machete {
  namespace draw {
    
    class FontChar : public MetaSprite {
    public:
      FontChar(const Vec2 & pivot, const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture);
      void SetXAdvance(float x) { advance.x = x; }
      void SetYAdvance(float y) { advance.y = y; }
      inline float GetXAdvance() { return advance.x; }
      inline float GetYAdvance() { return advance.y; }
      void SetAdvance(const Vec2 & adv) { advance = adv; }
      const Vec2 & GetAdvance() { return advance; }
      
    protected:
      Vec2 advance;
    };
    
    class Text : public Element {
    public:
      Text(int cap);
      ~Text();
      void Clear();
      void SetChar(int idx, FontChar* fchar);
      void SetLength(int len);
      
      void Invalidate();
      void Update(float time);
      void Draw(const Mat4 & matrix, DrawContext *ctx);
      
    protected:
      FontChar **chars;
      int cap;
      int len;
    };
    
    class Font {
    public:
      Font(const char* name, struct Tex * t);
      ~Font();
      
      FontChar* Get(int c);
      
      Text* Create(Str & str, int cap);
      void Change(Text *txt, Str & str);
      
    protected:
      void LoadFont();
      
      Mbd *font;
      Hash<int, FontChar*> chars;
      struct Tex *texture;
    };
    
  }
}
