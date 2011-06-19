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
    
    MetaSprite::MetaSprite(const Vec2 & pivot, const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture) {
      
      this->size = size;
      
      verts[0].pivot.x = pivot.x; verts[0].pivot.y = pivot.y;
      verts[1].pivot.x = pivot.x; verts[1].pivot.y = pivot.y;
      verts[2].pivot.x = pivot.x; verts[2].pivot.y = pivot.y;
      verts[3].pivot.x = pivot.x; verts[3].pivot.y = pivot.y;
      
      verts[0].vert.x = 0; verts[0].vert.y = 0; verts[0].vert.z = 0; verts[0].vert.w = 0;
      verts[1].vert.x = 0; verts[1].vert.y = -size.x; verts[1].vert.z = 0; verts[1].vert.w = 0;
      verts[2].vert.x = size.x; verts[2].vert.y = 0; verts[2].vert.z = 0; verts[2].vert.w = 0;
      verts[3].vert.x = size.x; verts[3].vert.y = -size.x; verts[3].vert.z = 0; verts[3].vert.w = 0;
      
      verts[0].uv.x = uv0.x; verts[0].uv.y = uv1.y;
      verts[1].uv.x = uv0.x; verts[1].uv.y = uv0.y;
      verts[2].uv.x = uv1.x; verts[2].uv.y = uv1.y;
      verts[3].uv.x = uv1.x; verts[3].uv.y = uv0.y;
      
      this->texture = texture;
      
      elems[0] = 0;
      elems[1] = 1;
      elems[2] = 2;
      elems[3] = 2;
      elems[4] = 1;
      elems[5] = 3;
    }
    
    MetaSprite::~MetaSprite() {
      
    }
    
    void MetaSprite::Draw(DrawContext *ctx, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation) {
      
      verts[0].offset.x = pos.x; verts[0].offset.y = pos.y;
      verts[1].offset.x = pos.x; verts[1].offset.y = pos.y;
      verts[2].offset.x = pos.x; verts[2].offset.y = pos.y;
      verts[3].offset.x = pos.x; verts[3].offset.y = pos.y;
      
      verts[0].color.x = color.x; verts[0].color.y = color.y; verts[0].color.z = color.z; verts[0].color.w = color.w;
      verts[1].color.x = color.x; verts[1].color.y = color.y; verts[1].color.z = color.z; verts[1].color.w = color.w;
      verts[2].color.x = color.x; verts[2].color.y = color.y; verts[2].color.z = color.z; verts[2].color.w = color.w;
      verts[3].color.x = color.x; verts[3].color.y = color.y; verts[3].color.z = color.z; verts[3].color.w = color.w;
      
      verts[0].scale.x = scale.x; verts[0].scale.y = scale.y;
      verts[1].scale.x = scale.x; verts[1].scale.y = scale.y;
      verts[2].scale.x = scale.x; verts[2].scale.y = scale.y;
      verts[3].scale.x = scale.x; verts[3].scale.y = scale.y;
      
      verts[0].rotation = rotation;
      verts[1].rotation = rotation;
      verts[2].rotation = rotation;
      verts[3].rotation = rotation;
      
      ctx->Draw(verts, 4, elems, 6, texture);
    }
    
    Element::Element() {
      rotation = 0;
      parent = NULL;
      visible = true;
      active = true;
    }
    
    Container::Container() {
      count = 0;
    }
   
    void Container::Add(Element *child) {
      childs.Append(child);
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
      element->Draw(ctx, position, scale, color, rotation);
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
    
  }
}
