//
//  meta.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "meta.h"

namespace machete {
  namespace draw {
    
    MetaSprite::MetaSprite(const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture) {
      
      this->size = size;
      this->uv0 = uv0;
      this->uv1 = uv1;
      
      verts[0].vert.x = 0; verts[0].vert.y = 0; verts[0].vert.z = 0;
      verts[1].vert.x = 0; verts[1].vert.y = -size.y; verts[1].vert.z = 0;
      verts[2].vert.x = size.x; verts[2].vert.y = 0; verts[2].vert.z = 0;
      verts[3].vert.x = size.x; verts[3].vert.y = -size.y; verts[3].vert.z = 0;
      
      verts[0].uv.x = uv0.x; verts[0].uv.y = uv0.y;
      verts[1].uv.x = uv0.x; verts[1].uv.y = uv1.y;
      verts[2].uv.x = uv1.x; verts[2].uv.y = uv0.y;
      verts[3].uv.x = uv1.x; verts[3].uv.y = uv1.y;
      
      vertsH[0] = verts[0];
      vertsH[1] = verts[1];
      vertsH[2] = verts[2];
      vertsH[3] = verts[3];
      vertsH[0].uv = verts[2].uv;
      vertsH[1].uv = verts[3].uv;
      vertsH[2].uv = verts[0].uv;
      vertsH[3].uv = verts[1].uv;
      
      vertsV[0] = verts[0];
      vertsV[1] = verts[1];
      vertsV[2] = verts[2];
      vertsV[3] = verts[3];
      vertsV[0].uv = verts[1].uv;
      vertsV[1].uv = verts[0].uv;
      vertsV[2].uv = verts[3].uv;
      vertsV[3].uv = verts[2].uv;
      
      vertsVH[0] = verts[0];
      vertsVH[1] = verts[1];
      vertsVH[2] = verts[2];
      vertsVH[3] = verts[3];
      vertsVH[0].uv = verts[3].uv;
      vertsVH[1].uv = verts[2].uv;
      vertsVH[2].uv = verts[1].uv;
      vertsVH[3].uv = verts[0].uv;
      
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
    
    void MetaSprite::Draw(DrawContext *ctx, Program *program, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) {
      
      Vtx *verts = this->verts;
      if (flipX && flipY) {
        verts = vertsVH;
      } else if (flipX) {
        verts = vertsH;
      } else if (flipY) {
        verts = vertsV;
      }

      verts[0].pivot.x = pivot.x; verts[0].pivot.y = pivot.y;
      verts[1].pivot.x = pivot.x; verts[1].pivot.y = pivot.y;
      verts[2].pivot.x = pivot.x; verts[2].pivot.y = pivot.y;
      verts[3].pivot.x = pivot.x; verts[3].pivot.y = pivot.y;

      verts[0].offset.x = pos.x; verts[0].offset.y = pos.y;
      verts[1].offset.x = pos.x; verts[1].offset.y = pos.y;
      verts[2].offset.x = pos.x; verts[2].offset.y = pos.y;
      verts[3].offset.x = pos.x; verts[3].offset.y = pos.y;
      
      verts[0].scale.x = scale.x; verts[0].scale.y = scale.y;
      verts[1].scale.x = scale.x; verts[1].scale.y = scale.y;
      verts[2].scale.x = scale.x; verts[2].scale.y = scale.y;
      verts[3].scale.x = scale.x; verts[3].scale.y = scale.y;
      
      verts[0].rotation = rotation;
      verts[1].rotation = rotation;
      verts[2].rotation = rotation;
      verts[3].rotation = rotation;
      
      ctx->Draw(program == NULL ? TheVertexShader : program, verts, 4, elems, 6, color, texture);
    }
    
    MetaSprite* MetaSpriteFromDrawContext(DrawContext *ctx) {
      return new MetaSprite(ctx->GetSize(), Vec2(0, 1), Vec2(1, 0), ctx->GetRenderTexture());
    }
    
    Vec2 Element::ZERO2(0, 0);
    Vec2 Element::ONE2(1, 1);
    unsigned long Element::NEXT_ID = 0;

    Element::Element() : color(1, 1, 1, 1), scale(1, 1) {
      rotation = 0;
      parent = NULL;
      data = NULL;
      visible = true;
      active = true;
      type = 0;
      program = NULL;
      objId = ++Element::NEXT_ID;
    }
    
    Rect2D Element::GetGlobalBounds() {
      Rect2D global = GetBounds();
      
      Element *par = parent;
      
      while (par != NULL) {
        global += par->position;
        
        par = par->parent;
      }
      
      return global;
    }
    
    void Element::SetShader(Program *program) {
      this->program = program;
    }

  }
}
