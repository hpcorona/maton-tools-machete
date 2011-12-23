//
//  draw.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "draw.h"
#include "../common/log.h"

using namespace machete::math;

namespace machete {
  namespace graphics {
    
    BufferMgr::BufferMgr() {
      ringIdx = -1;
      indices = NULL;
      vertexes = NULL;
      indexBuffer = 0;
      vertexBuffer = 0;
      
      CreateBuffers();
    }
    
    BufferMgr::~BufferMgr() {
      DestroyBuffers();
    }
    
    void BufferMgr::Next() {
      ringIdx++;
      
      if (ringIdx >= MAX_RING) {
        ringIdx = 0;
      }
      
      indices = indicesRing[ringIdx];
      vertexes = vertexesRing[ringIdx];
      indexBuffer = indexBufferRing[ringIdx];
      vertexBuffer = vertexBufferRing[ringIdx];
    }
    
    Vtx* BufferMgr::GetVtxArrayBuffer() {
      return vertexes;
    }
    
    GLushort* BufferMgr::GetIdxArrayBuffer() {
      return indices;
    }
    
    GLuint BufferMgr::GetVtxBuffer() {
      return vertexBuffer;
    }
    
    GLuint BufferMgr::GetIdxBuffer() {
      return indexBuffer;
    }

    GLuint BufferMgr::CreateVtxBuffer() const {
      GLuint buff;
      
      glGenBuffers(1, &buff);
      CheckGLError("glGenBuffers");
      glBindBuffer(GL_ARRAY_BUFFER, buff);
      CheckGLError("glBundBuffer");
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vtx) * MAX_VTX, NULL, GL_DYNAMIC_DRAW);
      CheckGLError("glBufferData");
      
      return buff;
    }
    
    GLuint BufferMgr::CreateIdxBuffer() const {
      GLuint buff;
      
      glGenBuffers(1, &buff);
      CheckGLError("glGenBuffers");
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff);
      CheckGLError("glBindBuffer");
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * MAX_IDX, NULL, GL_DYNAMIC_DRAW);
      CheckGLError("glBufferData");
      
      return buff;
    }
    
    void BufferMgr::DeleteBuffer(GLuint buffer) const {
      glDeleteBuffers(1, &buffer);
      CheckGLError("glDeleteBuffers");
    }
    
    void BufferMgr::CreateBuffers() {
      for (unsigned int i = 0; i < MAX_RING; i++) {
        vertexBufferRing[i] = CreateVtxBuffer();
        indexBufferRing[i] = CreateIdxBuffer();
      }
      
      ringIdx = 0;
      vertexes = vertexesRing[ringIdx];
      vertexBuffer = vertexBufferRing[ringIdx];
      
      indices = indicesRing[ringIdx];
      indexBuffer = indexBufferRing[ringIdx];
    }
    
    void BufferMgr::DestroyBuffers() {
      for (unsigned int i = 0; i < MAX_RING; i++) {
        DeleteBuffer(vertexBufferRing[i]);
        DeleteBuffer(indexBufferRing[i]);
      }
    }

    BufferMgr *TheBufferMgr = NULL;
    
    DrawContext::DrawContext(RenderTarget t) : color(0.5f, 0.5f, 0.5f, 1.0f), tint(1, 1, 1, 1) {
      if (TheBufferMgr == NULL) {
        TheBufferMgr = new BufferMgr();
      }
      
      target = t;
      framebuffer = 0;
      renderbuffer = 0;
      texture = 0;
      indices = NULL;
      vertexes = NULL;
      indexBuffer = 0;
      vertexBuffer = 0;
      idxCount = 0;
      vtxCount = 0;
      lastTexBind = 0;
      
#ifdef TARGET_ANDROID
      if (target == TargetTexture) {
#endif
        glGenFramebuffers(1, &framebuffer);
        CheckGLError("glGenFramebuffers");        
#ifdef TARGET_ANDROID
      }
#endif
      
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      CheckGLError("glBindFramebuffer");

      if (target == TargetScreen) {
#ifdef TARGET_IOS
        glGenRenderbuffers(1, &renderbuffer);
        CheckGLError("glGenRenderbuffers");
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        CheckGLError("glBindRenderbuffer");
#endif
      } else {
        color.x = 0; color.y = 0; color.z = 0; color.w = 0;
      }
    }
    
    IVec2 DrawContext::GetSize() const { return size; }
    
    void DrawContext::Initialize(int width, int height) {
      size.x = width;
      size.y = height;
      
      if (target == TargetTexture) {
        struct Tex *tex = TheTextureMgr->CreateTexture(width, height);
        
        texture = tex->id;
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
        CheckGLError("glFramebufferTexture2D");
#ifdef TARGET_IOS
      } else {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        CheckGLError("glFramebufferRenderbuffer");
#endif
      }
      
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      CheckGLError("glCheckFramebufferStatus");
      if (status != GL_FRAMEBUFFER_COMPLETE) {
        machete::common::Log("Frame buffer is not ready");
        return;
      }
      
      base = Mat4();
      base = base.Translate(-this->size.x / 2, this->size.y / 2, 0);
      
      renderer.Use();
      renderer.SetBase(base);
      
      glViewport(0, 0, size.x, size.y);
      CheckGLError("glViewport");

      renderer.ApplyOrtho(size.x, size.y);
      renderer.Unuse();
    }
    
    void DrawContext::Use() {
      lastTexBind = 0;
      
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      CheckGLError("glBindFramebuffer");

      glViewport(0, 0, size.x, size.y);
      CheckGLError("glViewport");

      renderer.Use();
      renderer.SetBase(base);
      
      glClearColor(color.x, color.y, color.z, color.w);
      CheckGLError("glClearColor");
    }
    
    void DrawContext::Unuse() {
      renderer.Unuse();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      CheckGLError("glBindFramebuffer");
    }
    
    void DrawContext::StartFrame() {
      glClear(GL_COLOR_BUFFER_BIT);
      CheckGLError("glClear");
      lastTexBind = 0;
      
      NextBuffers();
    }
    
    void DrawContext::ChangeModelView(const machete::math::Mat4 & mv) {
      renderer.SetModelView(mv);
    }
    
    void DrawContext::Draw(Vtx *verts, int vcount, unsigned short* elems, int ecount, const machete::math::Vec4 & tint, GLuint texId) {
      
      if (texId != lastTexBind || vtxCount + vcount > MAX_VTX || idxCount + ecount > MAX_IDX || this->tint.x != tint.x || this->tint.y != tint.y || this->tint.z != tint.z || this->tint.w != tint.w) {
        Draw();
      }

      lastTexBind = texId;
      this->tint = tint;

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
      CheckGLError("glBindBuffer ELEMENT");
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      CheckGLError("glBindBuffer ARRAY");
      
      glActiveTexture(GL_TEXTURE0);
      CheckGLError("glActiveTexture");
      glBindTexture(GL_TEXTURE_2D, lastTexBind);
      CheckGLError("glBindTexture");
      
      renderer.Upload(vertexes, vtxCount, indices, idxCount, tint);
      
      NextBuffers();
    }

    void DrawContext::EndFrame() {
      Draw();
      
      lastTexBind = 0;
      glBindTexture(GL_TEXTURE_2D, 0);
      CheckGLError("glBindTexture");
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      CheckGLError("glBindBuffer");
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      CheckGLError("glBindBuffer");
    }
    
    void DrawContext::NextBuffers() {
      vtxCount = 0;
      idxCount = 0;
      
      TheBufferMgr->Next();
      
      vertexes = TheBufferMgr->GetVtxArrayBuffer();
      vertexBuffer = TheBufferMgr->GetVtxBuffer();
      
      indices = TheBufferMgr->GetIdxArrayBuffer();
      indexBuffer = TheBufferMgr->GetIdxBuffer();
    }
    
    void DrawContext::SetClearColor(machete::math::Vec4 &color) {
      this->color = color;
    }
    
  }
}

