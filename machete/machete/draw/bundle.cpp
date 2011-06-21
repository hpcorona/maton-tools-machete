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
      
      delete bundle;
      
      bundle = NULL;
    }
    
    void Bundle::LoadAtlas() {
      Str name = bundle->Value("/Bundle/@atlasPath", &name);
      char nameChar[100];
      name.GetChars(nameChar, 100);
      
      atlas = new Mbd(nameChar);
      
      char textureName[100];
      Str texName = atlas->Value("/TextureAtlas/@imagePath");
      texName.GetChars(textureName, 100);
      
      tex = TheTextureMgr->LoadTexture(textureName);
      
      int total = atlas->Count("/TextureAtlas/SubTexture");
      for (int i = 0; i < total; i++) {
        Str stName = atlas->Value("/TextureAtlas/SubTexture[%1]/@name", i+1);
        float x = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@x", i+1);
        float y = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@y", i+1);
        float w = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@width", i+1);
        float h = atlas->FloatValue("/TextureAtlas/SubTexture[%1]/@height", i+1);
        float x1 = x + w;
        float y1 = y + h;
        
        MetaSprite *ms = new MetaSprite(Vec2(0, 0), Vec2(w, h), Vec2(x / tex.width, y / tex.height), Vec2(x1 / tex.width, y1 / tex.height), tex.id);
        
        images.Add(stName, ms);
      }
    }
    
    void Bundle::LoadSprites() {
      int sCount = bundle->Count("/Bundle/Sprite");
      for (int sIdx = 1; sIdx <= sCount; sIdx++) {
        int iCount = bundle->Count("/Bundle/Sprite[%1]/Image", sIdx);
        
        struct BdlSprite *sprite = new struct BdlSprite();
        sprite->images = new machete::data::Iterator<struct BdlImage*>();
        for (int iIdx = 1; iIdx <= iCount; iIdx++) {
          struct BdlImage *img = new struct BdlImage();
          Tree<Str, MetaSprite*> *node = images.Seek(bundle->Value("/Bundle/Sprite[%1]/Image[%2]/@name", sIdx, iIdx));
          
          if (node == NULL) {
            img->image = NULL;
          } else {
            img->image = node->GetValue();
          }
          
          img->position.x = bundle->FloatValue("/Bundle/Sprite[%1]/Image[%2]/@x", sIdx, iIdx);
          img->position.y = bundle->FloatValue("/Bundle/Sprite[%1]/Image[%2]/@y", sIdx, iIdx);
          img->alpha = bundle->FloatValue("/Bundle/Sprite[%1]/Image[%2]/@alpha", sIdx, iIdx) / 255.0f;
          img->rotation = bundle->FloatValue("/Bundle/Sprite[%1]/Image[%2]/@rotation", sIdx, iIdx);
          img->flipX = bundle->Value("/Bundle/Sprite[%1]/Image[%2]/@flipX", sIdx, iIdx) == "true";
          img->flipY = bundle->Value("/Bundle/Sprite[%1]/Image[%2]/@flipY", sIdx, iIdx) == "true";
          
          sprite->images->Append(img);
        }
        
        sprites.Add(bundle->Value("/Bundle/Sprite[%1]/@name", sIdx), sprite);
      }
    }
    
    void Bundle::LoadAnimations() {
      int aCount = bundle->Count("/Bundle/Animation");
      
      for (int aIdx = 1; aIdx <= aCount; aIdx++) {
        int fCount = bundle->Count("/Bundle/Animation[%1]/Frame", aIdx);
        
        struct BdlAnimation *animation = new struct BdlAnimation();
        animation->frames = new machete::data::Iterator<struct BdlTimed*>();
        for (int fIdx = 1; fIdx <= fCount; fIdx++) {
          struct BdlTimed *frame = new struct BdlTimed();
          Tree<Str, struct BdlSprite*> *node = sprites.Seek(bundle->Value("/Bundle/Animation[%1]/Frame[%2]/@sprite", aIdx, fIdx));
          
          if (node == NULL) {
            frame->sprite = NULL;
          } else {
            frame->sprite = node->GetValue();
          }
          
          frame->time = bundle->FloatValue("/Bundle/Animation[%1]/Frame[%2]/@time", aIdx, fIdx) / 100.0f;
          
          animation->frames->Append(frame);
        }

        animations.Add(bundle->Value("/Bundle/Animation[%1]/@name", aIdx), animation);

      }
    }
    
    void Bundle::LoadActors() {
      int xCount = bundle->Count("/Bundle/Actor");
      for (int xIdx = 1; xIdx <= xCount; xIdx++) {
        int aCount = bundle->Count("/Bundle/Actor[%1]/Action", xIdx);
        
        struct BdlActor* actor = new struct BdlActor();
        actor->actions = new machete::data::Iterator<struct BdlAction*>();
        for (int aIdx = 1; aIdx <= aCount; aIdx++) {
          struct BdlAction *action = new struct BdlAction();
          Tree<Str, struct BdlAnimation*> *node = animations.Seek(bundle->Value("/Bundle/Actor[%1]/Action[%2]/@animation", xIdx, aIdx));
          
          if (node == NULL) {
            action->animation = NULL;
          } else {
            action->animation = node->GetValue();
          }
          
          action->action = bundle->Value("/Bundle/Actor[%1]/Action[%2]/@name", xIdx, aIdx);
          
          actor->actions->Append(action);
        }
        
        actors.Add(bundle->Value("/Bundle/Actor[%1]/@name", xIdx), actor);
        
      }
    }
    
    void Bundle::LoadFonts() {
      int fCount = bundle->Count("/Bundle/Font");
      char fPath[100];
      
      for (int fIdx = 1; fIdx <= fCount; fIdx++) {
        Str faceName = bundle->Value("/Bundle/Font[%1]/@name", fIdx);
        Str facePath = bundle->Value("/Bundle/Font[%1]/@fontPath", fIdx);
        
        facePath.GetChars(fPath, 100);
        
        Font *face = new Font(fPath, tex);
        
        fonts.Add(faceName, face);
      }
    }
    
  }
}
