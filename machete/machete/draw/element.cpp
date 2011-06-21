//
//  element.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "element.h"

namespace machete {
  namespace draw {
    
    Container::Container() {
      count = 0;
    }
   
    void Container::Add(Element *child) {
      childs.Append(child);
      child->SetParent(this);
      count++;
    }
    
    void Container::Remove(Element *child) {
      childs.Reset();
      
      LinkedList<Element*> *prev = NULL, *current = NULL;
      
      prev = childs.GetRoot();
      while (childs.Next()) {
        current = childs.GetCurrent();
        
        if (current->GetValue() == child) {
          if (current == prev) {
            childs.RemoveRoot();
          } else {
            prev->SetNext(current->GetNext());
            delete current;
          }
          
          count--;
          break;
        }
        
        prev = current;
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
      
      if (parent != NULL) {
        parent->Invalidate();
      }
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
    
    void Container::Draw(const Mat4 & matrix, DrawContext *ctx) {
      if (count == 0) return;
      
      Mat4 mat = matrix;
      bool changed = false;
      
      if (position.x != 0 || position.y != 0 || scale.x != 1 || scale.y != 1 || rotation != 0) {
        mat.Translate(position.x, position.y, 0);
        mat.Scale(scale.x, scale.y, 0);
        mat.Rotate(rotation);
        changed = true;
        
        ctx->ChangeModelView(mat);
      }
      
      childs.Reset();
      
      while (childs.Next()) {
        Element *current = childs.GetCurrent()->GetValue();
        
        if (current->IsVisible()) {
          current->Draw(mat, ctx);
        }
      }
      
      if (changed) {
        ctx->ChangeModelView(matrix);
      }
    }
    
    Iterator<Element*> *Container::Iterator() {
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
      Container::Draw(matrix, context);
    }
    
    Drawing::Drawing(MetaElement *e) {
      element = e;
      bounds.size = element->GetSize();
    }
    
    void Drawing::Invalidate() {
      bounds.Clear();
      
      bounds.size = element->GetSize();
      bounds += position;
      bounds *= scale;
      
      if (parent != NULL) {
        parent->Invalidate();
      }
    }
    
    void Drawing::Update(float time) {
      // Nada que hacer
    }
    
    void Drawing::Draw(const Mat4 & matrix, DrawContext *ctx) {
      element->Draw(ctx, position, scale, color, rotation, flipX, flipY);
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
      
      if (e != NULL) {
        bounds += e->GetBounds();
      }
      
      bounds += position;
      bounds *= scale;
      
      if (parent != NULL) {
        parent->Invalidate();
      }
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
    
    void Animation::Draw(const Mat4 & matrix, DrawContext *ctx) {
      if (visible == false || current == NULL) return;
      
      current->GetValue()->GetElement()->Draw(matrix, ctx);
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
    
    inline bool Actor::IsFinished() const {
      if (current == NULL) return true;
      
      return current->IsFinished();
    }
    
    inline bool Actor::IsLoop() const {
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
    
    void Actor::Invalidate() {
      // TODO: Falta revisar esto
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
    
    void Actor::Draw(const Mat4 & matrix, DrawContext *ctx) {
      if (current != NULL) {
        current->Draw(matrix, ctx);
      }
    }
    
    void Actor::Restart() {
      if (current != NULL) {
        current->Restart();
      }
    }

  }
}
