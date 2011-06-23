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
#include "font.h"

using namespace machete::data;
using namespace machete::graphics;

namespace machete {
  namespace draw {
    struct BdlImage {
      BdlImage() { image = NULL; alpha = 1; rotation = 0; flipX = false; flipY = false; }
      
      MetaSprite *image;
      Vec2 position;
      float alpha;
      float rotation;
      bool flipX;
      bool flipY;
    };
    
    struct BdlSprite {
      BdlSprite() { imageCount = 0; images = NULL; }
      
      int imageCount;
      machete::data::Iterator<struct BdlImage*> *images;
    };
    
    struct BdlTimed {
      BdlTimed() { time = 0; sprite = NULL; }
      
      float time;
      struct BdlSprite *sprite;
    };
    
    struct BdlAnimation {
      BdlAnimation() { frames = NULL; }
      
      machete::data::Iterator<struct BdlTimed*> *frames;
    };
    
    struct BdlAction {
      BdlAction() { animation = NULL; }
      
      Str action;
      struct BdlAnimation *animation;
    };
    
    struct BdlActor {
      BdlActor() { actions = NULL; }
      
      machete::data::Iterator<struct BdlAction*> *actions;
    };
    
    class Bundle {
    public:
      Bundle(const char* name);

      MetaSprite* GetImage(const char* name) const;
      Drawing* NewImage(const char* name) const;
      Drawing* NewDrawing(struct BdlImage* bimg) const;
      Element* NewSprite(const char* name) const;
      Element* NewSprite(struct BdlSprite* spr) const;
      Animation* NewAnimation(const char* name) const;
      Animation* NewAnimation(struct BdlAnimation* banim) const;
      Actor* NewActor(const char* name) const;
      Actor* NewActor(struct BdlActor *bact) const;
      Font* GetFont(const char* name) const;
      
    protected:
      void LoadAtlas();
      void LoadSprites();
      void LoadAnimations();
      void LoadActors();
      void LoadFonts();
      
      Mbd *bundle;
      Mbd *atlas;
      struct Tex *tex;
      
      Hash<Str, MetaSprite*> images;
      Hash<Str, struct BdlSprite*> sprites;
      Hash<Str, struct BdlAnimation*> animations;
      Hash<Str, struct BdlActor*> actors;
      Hash<Str, Font*> fonts;
    };
    
  }
}
