//
//  font.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "font.h"

namespace machete {
  namespace draw {
    FontChar::FontChar(const Vec2 & pivot, const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture) : MetaSprite(pivot, size, uv0, uv1, texture) {
      advance.x = 0;
      advance.y = 0;
    }
    
    Text::Text(int cap) {
      this->cap = cap;
      
      Clear();
    }
    
    void Text::Clear() {
      chars = new FontChar*[cap];
      for (int i = 0; i < cap; i++) {
        chars[i] = NULL;
      }
      
      len = 0;
    }
    
    void Text::SetLength(int len) {
      this->len = len;
    }
    
    void Text::SetChar(int idx, FontChar* fchar) {
      chars[idx] = fchar;
    }
    
    void Text::Invalidate() {
      // TODO: Nada aún
    }
    
    void Text::Update(float time) {
      // Nada
    }
    
    void Text::Draw(const Mat4 & matrix, DrawContext *ctx) {
      float bx = 0;
      for (int i = 0; i < len; i++) {
        if (chars[i] == NULL) continue;
        
        chars[i]->Draw(ctx, Vec2(bx, 0), scale, color, rotation, false, false);
        
        bx += chars[i]->GetXAdvance();
      }
    }
    
    Font::Font(const char *name, struct Tex * t) {
      texture = t;
      
      font = new Mbd(name);
      
      LoadFont();
      
      delete font;
      
      font = NULL;
    }
    
    Text* Font::Create(Str & str, int cap) {
      Text *txt = new Text(cap);
      Change(txt, str);
      
      return txt;
    }
    
    void Font::Change(Text *txt, Str & str) {
      int cidx = 0;
      int ta = 0;
      int tidx = 0;
      
      while (cidx < str.Size()) {
        int c = str.CharAt(cidx);
        FontChar *fc = Get(c);
        
        ta = str.CharSizeAt(cidx);
        txt->SetChar(tidx++, fc);
        
        cidx += ta;
      }
      
      txt->SetLength(tidx);
    }
    
    void Font::LoadFont() {
      int height = font->IntValue("/font/common[1]/@lineHeight");
      int cCount = font->Count("/font/chars[1]/char");

      for (int cIdx = 1; cIdx <= cCount; cIdx++) {
        int c = font->IntValue("/font/chars[1]/char[%1]/@id", cIdx);
        float x = font->FloatValue("/font/chars[1]/char[%1]/@x", cIdx);
        float y = font->FloatValue("/font/chars[1]/char[%1]/@y", cIdx);
        float w = font->FloatValue("/font/chars[1]/char[%1]/@width", cIdx);
        float h = font->FloatValue("/font/chars[1]/char[%1]/@height", cIdx);
        float px = font->FloatValue("/font/chars[1]/char[%1]/@xoffset", cIdx);
        float py = font->FloatValue("/font/chars[1]/char[%1]/@yoffset", cIdx);
        float x1 = x + w;
        float y1 = y + h;
        float xa = font->FloatValue("/font/chars[1]/char[%1]/@xadvance", cIdx);
        
        FontChar *fchar = new FontChar(Vec2(px, py), Vec2(w, h), Vec2(x / texture->width, y / texture->height), Vec2(x1 / texture->width, y1 / texture->height), texture->id);
        
        fchar->SetXAdvance(xa);
        fchar->SetYAdvance(height);
        
        chars.Add(c, fchar);
      }
    }
    
    FontChar* Font::Get(int c) {
      Tree<int, FontChar*> *node = chars.Seek(c);
      
      if (node == NULL) {
        return NULL;
      }
      
      return node->GetValue();
    }

  }
}
