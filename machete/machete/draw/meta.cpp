//
//  meta.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "meta.h"

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
    
    void MetaSprite::Draw(DrawContext *ctx, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) {
      
      Vtx *verts = this->verts;
      if (flipX && flipY) {
        verts = vertsVH;
      } else if (flipX) {
        verts = vertsH;
      } else if (flipY) {
        verts = vertsV;
      }
      
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

    Element::Element() : color(1, 1, 1, 1), scale(1, 1) {
      rotation = 0;
      parent = NULL;
      data = NULL;
      visible = true;
      active = true;
    }

  }
}
