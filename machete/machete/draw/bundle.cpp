//
//  builder.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/19/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "bundle.h"

#include "../common/log.h"

using namespace machete::common;

namespace machete {
  namespace draw {
    
    Bundle::Bundle(const char* name) {
      bundle = new Mbd(name);
      tex = NULL;
      
      LoadAtlas();
      LoadSprites();
      LoadAnimations();
      LoadActors();
      LoadFonts();
      LoadFramed();
      LoadWidgets();
      
      delete bundle;
      
      bundle = NULL;
    }
    
    MetaSprite* Bundle::GetImage(const char* name) const {
      Tree<Str, MetaSprite*> *node = images.Seek(name);
      if (node == NULL) {
        return NULL;
      }
      
      return node->GetValue();
    }

    MetaWidget* Bundle::GetFramed(const char* name) const {
      Tree<Str, MetaWidget*> *node = framed.Seek(name);
      if (node == NULL) {
        return NULL;
      }
      
      return node->GetValue();
    }

    Drawing* Bundle::NewImage(const char* name) const {
      MetaSprite* spr = GetImage(name);
      
      if (spr == NULL) {
        return NULL;
      }
      
      Drawing *drw = new Drawing(spr);
      
      return drw;
    }
    
    Drawing* Bundle::NewDrawing(struct BdlImage* bimg) const {
      Drawing *drw = new Drawing(bimg->image);
      drw->SetPivot(bimg->position);
      drw->SetColor(1, 1, 1, bimg->alpha);
      drw->SetRotation(bimg->rotation);
      drw->SetFlipX(bimg->flipX);
      drw->SetFlipY(bimg->flipY);
      drw->Invalidate();
      
      return drw;
    }
    
    Element* Bundle::NewSprite(const char* name) const {
      Tree<Str, struct BdlSprite*> *node = sprites.Seek(name);
      
      if (node == NULL) {
        return NULL;
      }
      
      struct BdlSprite *mspr = node->GetValue();
      if (mspr == NULL) {
        return NULL;
      }
      
      return NewSprite(mspr);
    }
    
    Element* Bundle::NewSprite(struct BdlSprite* mspr) const {
      if (mspr == NULL) {
        return NULL;
      }
      
      if (mspr->imageCount == 1) {
        mspr->images->Reset();
        mspr->images->Next();
        
        struct BdlImage* bimg = mspr->images->GetCurrent()->GetValue();
        
        return NewDrawing(bimg);
      } else {
        mspr->images->Reset();
        
        Container *sprite = new Container();
        
        while (mspr->images->Next()) {
          struct BdlImage* bimg = mspr->images->GetCurrent()->GetValue();
          
          sprite->Add(NewDrawing(bimg));
        }
        
        return sprite;
      }
    }
    
    Animation *Bundle::NewAnimation(const char* name) const {
      Tree<Str, struct BdlAnimation*> *node = animations.Seek(name);
      
      if (node == NULL) {
        return NULL;
      }
      
      return NewAnimation(node->GetValue());
    }
    
    Animation *Bundle::NewAnimation(struct BdlAnimation *banim) const {
      if (banim == NULL) {
        return NULL;
      }
      
      banim->frames->Reset();
      
      Animation *animation = new Animation();
      while (banim->frames->Next()) {
        struct BdlTimed *mframe = banim->frames->GetCurrent()->GetValue();
        
        animation->Add(mframe->time, NewSprite(mframe->sprite));
      }
      
      return animation;
    }
    
    Actor *Bundle::NewActor(const char *name) const {
      Tree<Str, struct BdlActor*> *node = actors.Seek(name);
      
      if (node == NULL) {
        return NULL;
      }
      
      return NewActor(node->GetValue());
    }
    
    Actor *Bundle::NewActor(struct BdlActor *bact) const {
      if (bact == NULL) {
        return NULL;
      }
      
      bact->actions->Reset();
      
      Actor *actor = new Actor();
      while (bact->actions->Next()) {
        struct BdlAction *maction = bact->actions->GetCurrent()->GetValue();
        
        actor->Add(maction->action, NewAnimation(maction->animation));
      }
      
      return actor;
    }
    
    Font *Bundle::GetFont(const char* name) const {
      Tree<Str, Font*> *node = fonts.Seek(name);
      if (node == NULL) {
        return NULL;
      }
      
      return node->GetValue();
    }
    
    Widget *Bundle::NewWidget(const char *name) const {
      Tree<Str, struct BdlWidget*> *node = widgets.Seek(name);
      
      if (node == NULL) {
        Tree<Str, MetaWidget*> *fnode = framed.Seek(name);
        if (fnode == NULL) {
          return NULL;
        }
        
        MetaWidget *mw = fnode->GetValue();
        
        Widget *wdg = new Widget();
        wdg->Add("normal", mw);
        wdg->SetState("normal");
        
        if (mw->IsVertical() && !mw->IsHorizontal()) {
          wdg->SetDisplay(WidgetVertical);
        } else if (mw->IsHorizontal() && !mw->IsVertical()) {
          wdg->SetDisplay(WidgetHorizontal);
        }
        
        wdg->SetSize(mw->GetSize());
        
        return wdg;
      }
      
      return NewWidget(node->GetValue());
    }
    
    Widget *Bundle::NewWidget(struct BdlWidget *bwdg) const {
      if (bwdg == NULL) {
        return NULL;
      }
      
      bwdg->states->Reset();
      
      bool first = true;
      
      Widget *widget = new Widget();
      while (bwdg->states->Next()) {
        struct BdlWidgetState *mstate = bwdg->states->GetCurrent()->GetValue();
        
        widget->Add(mstate->state, mstate->framed);
        
        if (first) {
          first = false;
          
          widget->SetSize(mstate->framed->GetSize());
        }
      }
      
      return widget;
    }

    Button *Bundle::NewButton(const char *name) const {
      Tree<Str, struct BdlWidget*> *node = widgets.Seek(name);
      
      if (node == NULL) {
        return NULL;
      }
      
      return NewButton(node->GetValue());
    }
    
    Button *Bundle::NewButton(struct BdlWidget *bwdg) const {
      if (bwdg == NULL) {
        return NULL;
      }
      
      bwdg->states->Reset();
      
      bool first = true;
      
      Button *widget = new Button();
      while (bwdg->states->Next()) {
        struct BdlWidgetState *mstate = bwdg->states->GetCurrent()->GetValue();
        
        widget->Add(mstate->state, mstate->framed);
        
        if (first) {
          first = false;
          
          widget->SetSize(mstate->framed->GetSize());
        }
      }
      
      widget->SetState("normal");
      
      return widget;
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
        
        Vec2 uv0;
        uv0.x = x / tex->width;
        uv0.y = y / tex->height;
        
        Vec2 uv1;
        uv1.x = x1 / tex->width;
        uv1.y = y1 / tex->height;
        
#ifdef TARGET_ANDROID
        uv0.y = 1 - uv0.y;
        uv1.y = 1 - uv1.y;
#endif
        
        MetaSprite *ms = new MetaSprite(Vec2(w, h), uv0, uv1, tex);
        
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
          
          Str flipX = bundle->Value("/Bundle/Sprite[%1]/Image[%2]/@flipX", sIdx, iIdx);
          Str flipY = bundle->Value("/Bundle/Sprite[%1]/Image[%2]/@flipY", sIdx, iIdx);
          img->flipX = flipX == "true";
          img->flipY = flipY == "true";
          
          sprite->images->Append(img);
        }
        
        sprite->imageCount = iCount;
        
        Str spriteName = bundle->Value("/Bundle/Sprite[%1]/@name", sIdx);
        sprites.Add(spriteName, sprite);
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
          
          frame->time = bundle->FloatValue("/Bundle/Animation[%1]/Frame[%2]/@time", aIdx, fIdx) / 1000.0f;
          
          animation->frames->Append(frame);
        }

        Str animName = bundle->Value("/Bundle/Animation[%1]/@name", aIdx);
        animations.Add(animName, animation);
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
        
        Str actorName = bundle->Value("/Bundle/Actor[%1]/@name", xIdx);
        actors.Add(actorName, actor);
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
    
    void Bundle::LoadFramed() {
      int fCount = bundle->Count("/Bundle/Framed");
      for (int fIdx = 1; fIdx <= fCount; fIdx++) {
        Str name = bundle->Value("/Bundle/Framed[%1]/@name", fIdx);
        
        float top = bundle->FloatValue("/Bundle/Framed[%1]/@top", fIdx);
        float left = bundle->FloatValue("/Bundle/Framed[%1]/@left", fIdx);
        float right = bundle->FloatValue("/Bundle/Framed[%1]/@right", fIdx);
        float bottom = bundle->FloatValue("/Bundle/Framed[%1]/@bottom", fIdx);
        
        MetaSprite *image = NULL;
        
        Tree<Str, MetaSprite*> *node = images.Seek(name);
        if (node != NULL) {
          image = node->GetValue();
        }
        
        if (image == NULL) {
          continue;
        }
        
        Vec2 topLeft(left, top);
        Vec2 bottomRight(right, bottom);
        
        MetaWidget *mw = new MetaWidget(image->GetSize(), image->GetUV0(), image->GetUV1(), topLeft, bottomRight, image->GetTexture());
        
        framed.Add(name, mw);
      }
    }
    
    void Bundle::LoadWidgets() {
      int xCount = bundle->Count("/Bundle/Widget");
      for (int xIdx = 1; xIdx <= xCount; xIdx++) {
        int aCount = bundle->Count("/Bundle/Widget[%1]/State", xIdx);
        
        struct BdlWidget* widget = new struct BdlWidget();
        widget->states = new machete::data::Iterator<struct BdlWidgetState*>();
        for (int aIdx = 1; aIdx <= aCount; aIdx++) {
          struct BdlWidgetState *state = new struct BdlWidgetState();
          
          MetaWidget *fra = NULL;
          Tree<Str, MetaWidget*> *node = framed.Seek(bundle->Value("/Bundle/Widget[%1]/State[%2]/@image", xIdx, aIdx));
          if (node != NULL) {
            fra = node->GetValue();
          }
          
          if (fra == NULL) {
            state->framed = NULL;
          } else {
            state->framed = fra;
          }
          
          state->state = bundle->Value("/Bundle/Widget[%1]/State[%2]/@name", xIdx, aIdx);
          
          widget->states->Append(state);
        }
        
        Str widgetName = bundle->Value("/Bundle/Widget[%1]/@name", xIdx);
        widgets.Add(widgetName, widget);
      }
    }
    
    ActorButton* Bundle::NewActorButton(const char* name) const {
      return new ActorButton(NewActor(name));
    }
    
  }
}
