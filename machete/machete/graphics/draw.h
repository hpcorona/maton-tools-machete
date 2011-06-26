//
//  draw.h
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include "shader.h"

#define MAX_RING 5
#define MAX_SPR 50
#define MAX_VTX (MAX_SPR*4)
#define MAX_IDX (MAX_SPR*6)

namespace machete {
  namespace graphics {
    
    enum RenderTarget {
      TargetScreen,
      TargetTexture
    };
    
    class DrawContext {
    public:
      DrawContext(RenderTarget target);
      void Initialize(int width, int height);
      ~DrawContext() {}
      
      void Use();
      void Unuse();
      
      void StartFrame();
      void EndFrame();

      void ChangeModelView(const machete::math::Mat4 & mv);
      void Draw(Vtx *verts, int vcount, unsigned short* elems, int ecount, GLuint texId);
      void Draw();
      
      machete::math::IVec2 GetSize() const;
      unsigned int GetRenderTexture() const;
      
      void CreateBuffers();
      void DestroyBuffers();
      
      // Creo que esto no deberia de ir aqui
      unsigned int CreateVtxBuffer(Vtx* verts) const;
      unsigned int CreateIdxBuffer(unsigned short* idxs) const;
      void DeleteBuffer(unsigned int buffer) const;

    private:
      VtxRender renderer;
      
      RenderTarget target;
      
      GLuint framebuffer;
      GLuint renderbuffer;
      GLuint texture;
      
      GLushort indicesRing[MAX_RING][MAX_IDX];
      GLushort* indices;
      
      Vtx vertexesRing[MAX_RING][MAX_VTX];
      Vtx* vertexes;
      
      GLuint indexBuffer;
      GLuint indexBufferRing[MAX_RING];
      
      GLuint vertexBuffer;
      GLuint vertexBufferRing[MAX_RING];
      
      unsigned int ringIdx;
      unsigned int idxCount;
      unsigned int vtxCount;
      
      machete::math::IVec2 size;
      machete::math::Mat4 base;
      
      GLuint lastTexBind;
    };
    
    class IDrawable {
    public:
      virtual void Draw(const machete::math::Vec2 & pos, float rotation, const machete::math::Vec2 & scale, const machete::math::Vec4 & color, DrawContext* context) = 0;
      virtual machete::math::Rect2D GetBounds() = 0;
    };

  }
}
