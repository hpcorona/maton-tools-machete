//
//  builder.h
//  machete
//
//  Created by Hilario Perez Corona on 6/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "element.h"
#include "../data/mbd.h"

using namespace machete::data;
using namespace machete::graphics;

namespace machete {
  namespace draw {
    struct BdlImage {
      MetaSprite *image;
      Vec2 position;
      float alpha;
      float rotation;
      bool flipX;
      bool flipY;
    };
    
    struct BdlSprite {
      machete::data::Iterator<struct BdlImage*> *images;
    };
    
    struct BdlTimed {
      float time;
      struct BdlSprite *sprite;
    };
    
    struct BdlAnimation {
      machete::data::Iterator<struct BdlTimed*> *sprites;
    };
    
    struct BdlAction {
      Str action;
      struct BdlAnimation *animation;
    };
    
    struct BdlActor {
      machete::data::Iterator<struct BdlAction*> *actions;
    };
    
    struct BdlFont {
      Hash<int, MetaSprite*> *chars;
    };
    
    class Bundle {
    public:
      Bundle(const char* name);

      
    protected:
      void LoadAtlas();
      void LoadSprites();
      void LoadAnimations();
      void LoadActors();
      void LoadFonts();
      
      Mbd *bundle;
      Mbd *atlas;
      unsigned int texture;
      
      Hash<Str, MetaSprite*> images;
      Hash<Str, struct BdlSprite*> sprites;
      Hash<Str, struct BdlAnimation*> animations;
      Hash<Str, struct BdlActor*> actors;
      Hash<Str, struct BdlFont*> fonts;
    };
    
  }
}
