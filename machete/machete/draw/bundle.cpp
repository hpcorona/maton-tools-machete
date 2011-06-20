//
//  builder.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "bundle.h"

namespace machete {
  namespace draw {
    
    Bundle::Bundle(const char* name) {
      bundle = new Mbd(name);
      
      LoadAtlas();
      LoadSprites();
      LoadAnimations();
      LoadActors();
      LoadFonts();
    }
    
    void Bundle::LoadAtlas() {
      Str name = bundle->Value("/Bundle/@atlasPath", &name);
      char nameChar[100];
      name.GetChars(nameChar, 100);
      
      atlas = new Mbd(nameChar);
      
      char textureName[100];
      Str texName = atlas->Value("/TextureAtlas/@imagePath");
      texName.GetChars(textureName, 100);
      
      Tex t = TheTextureMgr->LoadTexture(textureName);
      
      int total = atlas->Count("/TextureAtlas/SubTexture");
      for (int i = 0; i < total; i++) {
        Str stName = atlas->Value("/TextureAtlas/SubTexture[%1]/@name", i);
        float x = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@x", i);
        float y = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@y", i);
        float w = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@width", i);
        float h = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@height", i);
        float x1 = x + w;
        float y1 = y + h;
        
        MetaSprite *ms = new MetaSprite(Vec2(0, 0), Vec2(w, h), Vec2(x / t.width, y / t.height), Vec2(x1 / t.width, y1 / t.height), t.id);
        
        images.Add(stName, ms);
      }
    }
    
    void Bundle::LoadSprites() {
      /*
       <Sprite name="arrowRight1">
       <Image name="arrowLeft" x="-24" y="-24" alpha="255" rotation="0.0" flipX="false" flipY="false" />
       */
    }
    
    void Bundle::LoadAnimations() {
      /*
       <Animation name="arrowDown">
       <Frame sprite="arrowDown1" time="100" />
       */
    }
    
    void Bundle::LoadActors() {
      /*
       <Actor name="arrowLeft">
       <Action name="default" animation="arrowLeft" />
       */
    }
    
    void Bundle::LoadFonts() {
      /*
       <Font name="default" fontPath="default.fnt" />
       */
    }
    
  }
}