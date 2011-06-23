//
//  meta.h
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "../graphics/draw.h"
#include "../data/lists.h"

using namespace machete::graphics;
using namespace machete::math;
using namespace machete::data;

namespace machete {
  namespace draw {
    
    class MetaElement {
    public:
      virtual void Draw(DrawContext *ctx, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) = 0;
      virtual Vec2 & GetSize() = 0;
    };
    
    class MetaSprite : public MetaElement {
    public:
      
      MetaSprite(const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture);
      ~MetaSprite();
      
      void Draw(DrawContext *ctx, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY);
      Vec2 & GetSize() { return size; }
      
    protected:
      Vtx verts[4], vertsV[4], vertsH[4], vertsVH[4];
      unsigned short elems[6];
      unsigned int texture;
      Vec2 size;
    };
    
    class Element {
    public:
      Element();
      
      void SetPosition(const Vec2 & pos) { position.x = pos.x; position.y = pos.y; }
      void SetPosition(float x, float y) { position.x = x; position.y = y; }
      void SetPivot(float x, float y) { this->pivot.x = x; this->pivot.y = y; }
      void SetPivot(const Vec2 & pivot) { this->pivot.x = pivot.x; this->pivot.y = pivot.y; }
      void SetRotation(float rotation) { this->rotation = rotation; }
      void SetScale(float x, float y) { scale.x = x; scale.y = y; }
      void SetScale(const Vec2 & scale) { this->scale.x = scale.x; this->scale.y = scale.y; }
      void SetColor(float r, float g, float b, float a) { color.x = r; color.y = g; color.z = b; color.w = a; }
      void SetColor(const Vec4 & color) { this->color.x = color.x; this->color.y = color.y; this->color.z = color.z; this->color.w = color.w; }
      Vec2 & GetPosition() { return position; }
      Vec2 & GetPivot() { return pivot; }
      Vec2 & GetScale() { return scale; }
      Vec4 & GetColor() { return color; }
      Rect2D & GetBounds() { return bounds; }
      void SetActive(bool active) { this->active = active; }
      bool IsActive() { return active; }
      void SetVisible(bool visible) { this->visible = visible; }
      bool IsVisible() { return visible; }
      void SetData(void *data) { this->data = data; }
      void* GetData() { return data; }
      void Remove(Element *child) { /* Nada */ }
      void RemoveFromParent() { if (parent != NULL) { parent->Remove(this); } parent = NULL; }
      void SetParent(Element *parent) { this->parent = parent; }
      Element* GetParent() { return parent; }
      void Disable() { active = false; visible = false; }
      void Enable() { active = true; visible = true; }
      
      virtual void Invalidate() = 0;
      virtual void Update(float time) = 0;
      virtual void Draw(const Mat4 & matrix, DrawContext *ctx) = 0;
      
    protected:
      bool active;
      bool visible;
      Vec2 position;
      Vec2 pivot;
      float rotation;
      Vec2 scale;
      Vec4 color;
      Element *parent;
      Rect2D bounds;
      void *data;
    };

  }
}
