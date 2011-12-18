//
//  element.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/17/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "element.h"

namespace machete {
  namespace draw {
    
    Container::Container() {
      count = 0;
      culled = false;
    }
    
    void Container::Add(Element *child) {
      childs.Append(child);
      child->SetParent(this);
      count++;
    }
    
    void Container::Remove(Element *child) {
      childs.Reset();
      
      LinkedList<Element*> *current = NULL;
      
      while (childs.Next()) {
        current = childs.GetCurrent();
        
        if (current->GetValue() == child) {
          childs.RemoveCurrent(true);
          
          count--;
          break;
        }
      }
    }
    
    void Container::RemoveAllChilds(bool del) {
      while (childs.Count() > 0) {
        if (del) {
          childs.Reset();
          childs.Next();
          Element *elm = childs.GetCurrent()->GetValue();
          delete elm;
          
          childs.GetCurrent()->SetValue(NULL);
        }
        childs.RemoveRoot();
      }
    }
    
    void Container::Switch(Element *child, Element *elem) {
      childs.Reset();
      
      LinkedList<Element*> *prev = NULL, *current = NULL;
      
      prev = childs.GetRoot();
      while (childs.Next()) {
        current = childs.GetCurrent();
        
        if (current->GetValue() == child) {
          elem->SetParent(child->GetParent());
          current->GetValue()->SetParent(NULL);
          current->SetValue(elem);
          break;
        }
        
        prev = current;
      }
    }
    
    void Container::Invalidate() {
      bounds.Clear();
      
      childs.Reset();
      Element *current = NULL;
      
      while (childs.Next()) {
        current = childs.GetCurrent()->GetValue();
        bounds += current->GetBounds();
      }
      
      // TODO: Aplicar Posicion, Rotación, Escala
      bounds += position;
      bounds *= scale;
    }
    
    void Container::Update(float time) {
      childs.Reset();
      
      while (childs.Next()) {
        Element *current = childs.GetCurrent()->GetValue();
        
        if (current->IsActive()) {
          current->Update(time);
        }
      }
    }
    
    void Container::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (count == 0) return;
      
      Mat4 mat = matrix;
      bool changed = false;
      
      if (scale.x != 1 || scale.y != 1 || rotation != 0) {
        
        ctx->Draw();
        
        ctx->ChangeModelView(Mat4().Translate(position.x + pos.x, position.y + pos.y, 0).Scale(scale.x, scale.y, 0).Rotate(rotation).Pointer());
        changed = true;
      }
      
      childs.Reset();
      
      Vec4 NewColor = this->color * color;
      Vec2 Position = position + pos;
      
      while (childs.Next()) {
        Element *current = childs.GetCurrent()->GetValue();
        
        if (current->IsVisible()) {
          if (culled) {
            Rect2D bnds = current->GetGlobalBounds();
            
            if (!cullViewport.Intersects(bnds)) {
              continue;
            }
          }
          
          if (changed) {
            current->Draw(mat, ZERO2, NewColor, ctx);
          } else {
            current->Draw(mat, Position, NewColor, ctx);
          }
        }
      }
      
      if (changed) {
        ctx->Draw();
        ctx->ChangeModelView(matrix);
      }
    }
    
    bool Container::TouchEvent(machete::input::Touch *touch) {
      if (touch->owner != this) {
        if (!active || !visible) return false;
      
        childs.Reset();
        while (childs.Previous()) {
          if (childs.GetCurrent()->GetValue()->TouchEvent(touch)) {
            return true;
          }
        }
      }
      
      return false;
    }
    
    void Container::SetCulling(bool culling) {
      this->culled = culling;
    }
    
    bool Container::IsCulling() const {
      return culled;
    }
    
    void Container::SetCullViewport(Rect2D & viewport) {
      cullViewport = viewport;
    }
    
    Iterator<Element*>* Container::ChildIterator() {
      if (count == 0) {
        return NULL;
      }
      
      return new machete::data::Iterator<Element*>(childs.GetRoot());
    }
    
    Root::Root(DrawContext *ctx) {
      context = ctx;
      matrix.Identity();
    }
    
    void Root::Draw() {
      context->Use();
      context->StartFrame();
      
      context->ChangeModelView(matrix);
      Container::Draw(matrix, position, color, context);
      
      context->EndFrame();
      context->Unuse();
    }
    
    Drawing::Drawing(MetaElement *e) {
      element = e;
      bounds.pos = position + pivot;
      
      if (element != NULL) {
        bounds.size = element->GetSize();
      }
      
      flipX = false;
      flipY = false;
    }
    
    void Drawing::Invalidate() {
      bounds.Clear();
      
      if (element == NULL) return;
      
      bounds.size = element->GetSize();
      bounds.pos += position + pivot;
      bounds *= scale;
    }
    
    void Drawing::Update(float time) {
      // Nada que hacer
    }
    
    void Drawing::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (element == NULL) return;
      
      Vec2 Position = position + pos;
      Vec4 Color = this->color * color;
      
      element->Draw(ctx, pivot, Position, scale, Color, rotation, flipX, flipY);
    }
    
    Animation::Animation() {
      loop = true;
      finished = false;
      time = 0;
      currTime = 0;
      current = NULL;
    }
    
    void Animation::Add(float time, Element *child) {
      TimedElement *te = new TimedElement(time, child);
      frames.Append(te);
      
      totalTime += time;
    }
    
    void Animation::Invalidate() {
      bounds.Clear();
      
      Element *e = NULL;
      
      if (current != NULL) {
        e = current->GetValue()->GetElement();
      }
      
      if (e == NULL && frames.GetRoot() != NULL && frames.GetRoot()->IsEmpty() == false) {
        e = frames.GetRoot()->GetValue()->GetElement();
      }
      
      if (e != NULL) {
        bounds += e->GetBounds();
      }
      
      bounds += position;
      bounds *= scale;
    }
    
    void Animation::Update(float time) {
      if (IsActive() == false) return;
      
      if (currTime - time <= 0) {
        if (frames.Next()) {
          this->time += currTime;
          time -= currTime;
          current = frames.GetCurrent();
          currTime = current->GetValue()->GetTime();
          
          if (time > currTime) {
            this->time += currTime;
            time -= currTime;
            Update(time);
          } else {
            currTime -= time;
            this->time += time;
          }
          
          finished = false;
        } else {
          finished = true;
          
          if (loop) {
            time -= currTime;
            Restart();
            
            Update(time);
          }
        }
      } else {
        this->time += time;
        currTime -= time;
        finished = false;
      }
    }
    
    void Animation::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (visible == false || current == NULL) return;
      
      Vec2 Position = position + pos;
      Vec4 Color = this->color * color;
      
      Element *elem = current->GetValue()->GetElement();
      if (elem != NULL) {
        elem->Draw(matrix, Position, Color, ctx);
      }
    }
    
    void Animation::Restart() {
      time = 0;
      currTime = 0;
      current = NULL;
      finished = false;
      frames.Reset();
    }
    
    Actor::Actor() {
      fallback = NULL;
      current = NULL;
    }
    
    void Actor::Add(const Str & action, Animation *anim) {
      actions.Add(action, anim);
    }
    
    bool Actor::IsFinished() const {
      if (current == NULL) return true;
      
      return current->IsFinished();
    }
    
    bool Actor::IsLoop() const {
      if (current == NULL) return false;
      
      return current->IsLoop();
    }
    
    void Actor::SetFallback(const char* name) {
      Tree<Str, Animation*> *node = actions.Seek(name);
      if (node == NULL) {
        fallback = NULL;
      }
      
      fallback = node->GetValue();
    }
    
    void Actor::SetLoop(bool loop) {
      if (current == NULL) return;
      
      current->SetLoop(loop);
    }
    
    bool Actor::Play(const char* name) {
      Tree<Str, Animation*> *node = actions.Seek(name);
      if (node == NULL) {
        return false;
      }
      
      current = node->GetValue();
      
      if (current == NULL) {
        return false;
      }
      
      current->Restart();
      
      return true;
    }
    
    bool Actor::Play(const char* name, const char* fallback) {
      if (!Play(name, false)) {
        return false;
      }
      
      SetFallback(fallback);
      
      return true;
    }
    
    bool Actor::Play(const char *name, bool loop) {
      if (Play(name) == false) {
        return false;
      }
      
      SetLoop(loop);
      
      return true;
    }
    
    void Actor::Invalidate() {
      bounds.Clear();
      
      Element *e = NULL;
      
      if (current != NULL) {
        e = current;
      }
      
      if (e != NULL) {
        e->Invalidate();
        bounds += e->GetBounds();
      }
      
      bounds += position;
      bounds *= scale;
    }
    
    void Actor::Update(float time) {
      if (current == NULL) return;
      
      if (current->IsFinished() && !current->IsLoop() && fallback != NULL) {
        current = fallback;
        current->Restart();
      }
      
      if (current != NULL) {
        current->Update(time);
      }
    }
    
    void Actor::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (current != NULL) {
        Vec2 Position = position + pos;
        Vec4 Color = this->color * color;
        
        current->Draw(matrix, Position, Color, ctx);
      }
    }
    
    void Actor::Restart() {
      if (current != NULL) {
        current->Restart();
      }
    }
    
    Dynamic::Dynamic(int texWidth, int texHeight, Vec2 & size) : Root(NULL) {
      usingTexture = false;
      dynamic = CreateDrawContext(machete::graphics::TargetTexture, texWidth, texHeight);
      texture = new MetaSprite(size, Vec2(0, 1), Vec2(1, 0), dynamic->GetRenderTexture());
    }
    
    Dynamic::~Dynamic() {
      delete texture;
      delete dynamic;
      
      dynamic = NULL;
    }
    
    void Dynamic::Draw() {
      dynamic->Use();
      dynamic->StartFrame();
      
      dynamic->ChangeModelView(matrix);
      
      Vec2 negative(-position.x, -position.y);
      
      Container::Draw(matrix, negative, color, dynamic);
      
      dynamic->EndFrame();
      dynamic->Unuse();
    }
    
    void Dynamic::Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) {
      if (usingTexture) {
        if (texture != NULL) {
          Vec4 NewColor = this->color * color;
          Vec2 Position = position + pos;
          
          texture->Draw(ctx, pivot, Position, scale, NewColor, rotation, false, false);
        }
      } else {
        Container::Draw(matrix, pos, color, ctx);
      }
    }

    void Dynamic::Draw(DrawContext *ctx) {
      if (usingTexture) {
        if (texture != NULL) {
          texture->Draw(ctx, pivot, position, scale, color, rotation, false, false);
        }
      } else {
        Root::context = ctx;
        Root::Draw();
      }
    }
    
    void Dynamic::SetDrawContext(machete::graphics::DrawContext *ctx) {
      Root::context = ctx;
    }
    
  }
}
