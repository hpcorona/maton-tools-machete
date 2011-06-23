//
//  draw.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "draw.h"
#include "../common/log.h"

using namespace machete::math;

namespace machete {
  namespace graphics {
    
    DrawContext::DrawContext(RenderTarget t) {
      target = t;
      
      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

      if (target == TargetScreen) {
        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
      }
    }
    
    IVec2 DrawContext::GetSize() const { return size; }
    
    void DrawContext::Initialize(int width, int height) {
      size.x = width;
      size.y = height;
      
      if (target == TargetTexture) {
        struct Tex *tex = TheTextureMgr->CreateTexture(width, height);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
      } else {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
      }
      
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE) {
        machete::common::Log("Frame buffer is not ready");
        exit(1);
      }
      
      
      CreateBuffers();

      base = Mat4();
      base = base.Translate(-this->size.x / 2, this->size.y / 2, 0);
      
      renderer.Use();
      renderer.SetBase(base);
      
      glViewport(0, 0, size.x, size.y);

      renderer.ApplyOrtho(size.x, size.y);
    }
    
    void DrawContext::Use() {
      lastTexBind = 0;
      
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      //glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
      //CheckGLError();

      //glBindFramebuffer(1, framebuffer);
      /*
       if (target == TargetScreen) {
       glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
       }
       */

      glViewport(0, 0, size.x, size.y);

      renderer.Use();
      renderer.SetBase(base);
      
      if (target == TargetScreen) {
        glClearColor(0.5f, 0.5f, 0.5f, 1);
      } else {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      }
    }
    
    void DrawContext::Unuse() {
      renderer.Unuse();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      //glBindRenderbuffer(GL_RENDERBUFFER, 0);
      //CheckGLError();
    }
    
    void DrawContext::StartFrame() {
      glClear(GL_COLOR_BUFFER_BIT);
      lastTexBind = 0;
      idxCount = 0;
      vtxCount = 0;
      vertexBuffer = vertexBufferRing[ringIdx];
      indexBuffer = indexBufferRing[ringIdx];
    }
    
    void DrawContext::ChangeModelView(const machete::math::Mat4 & mv) {
      renderer.SetModelView(mv);
    }
    
    void DrawContext::Draw(Vtx *verts, int vcount, unsigned short* elems, int ecount, GLuint texId) {
      
      if (texId != lastTexBind) {
        lastTexBind = texId;
        
        Draw();
      }
      
      unsigned int vtxBase = vtxCount;
      
      for (int i = 0; i < vcount; i++) {
        vertexes[vtxCount++] = verts[i];
      }
      
      for (int i = 0; i < ecount; i++) {
        indices[idxCount++] = elems[i] + vtxBase;
      }
    }
    
    void DrawContext::Draw() {
      if (vtxCount == 0) return;

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, lastTexBind);
      
      renderer.Upload(vertexes, vtxCount, indices, idxCount);
      
      vtxCount = 0;
      idxCount = 0;
      
      ringIdx++;
      if (ringIdx >= MAX_RING) {
        ringIdx = 0;
      }
      
      vertexes = vertexesRing[ringIdx];
      vertexBuffer = vertexBufferRing[ringIdx];
      
      indices = indicesRing[ringIdx];
      indexBuffer = indexBufferRing[ringIdx];
    }
    
    void DrawContext::EndFrame() {
      Draw();
      
      lastTexBind = 0;
      glBindTexture(GL_TEXTURE_2D, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    GLuint DrawContext::CreateVtxBuffer(Vtx* verts) const {
      GLuint buff;
      
      glGenBuffers(1, &buff);
      glBindBuffer(GL_ARRAY_BUFFER, buff);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vtx) * MAX_VTX, NULL, GL_DYNAMIC_DRAW);
      
      return buff;
    }

    GLuint DrawContext::CreateIdxBuffer(unsigned short* idxs) const {
      GLuint buff;
      
      glGenBuffers(1, &buff);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * MAX_IDX, NULL, GL_DYNAMIC_DRAW);
      
      return buff;
    }

    void DrawContext::DeleteBuffer(GLuint buffer) const {
      glDeleteBuffers(1, &buffer);
    }
    
    void DrawContext::CreateBuffers() {
      for (unsigned int i = 0; i < MAX_RING; i++) {
        vertexBufferRing[i] = CreateVtxBuffer(vertexesRing[i]);
        indexBufferRing[i] = CreateIdxBuffer(indicesRing[i]);
      }
      
      ringIdx = 0;
      vertexes = vertexesRing[ringIdx];
      vertexBuffer = vertexBufferRing[ringIdx];
      
      indices = indicesRing[ringIdx];
      indexBuffer = indexBufferRing[ringIdx];
      
      vtxCount = 0;
      idxCount = 0;
    }
    
  }
}
