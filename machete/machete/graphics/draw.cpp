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
      renderer = NULL;
      
#ifdef TARGET_ANDROID
      if (target == TargetTexture) {
#endif
#ifndef OPENGL_11
        glGenFramebuffers(1, &framebuffer);
        CheckGLError("glGenFramebuffers");
#else
        glGenFramebuffersOES(1, &framebuffer);
        CheckGLError("glGenFramebuffersOES");
#endif
#ifdef TARGET_ANDROID
      }
#endif
      
#ifndef OPENGL_11
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      CheckGLError("glBindFramebuffer");
#else
      glBindFramebufferOES(GL_FRAMEBUFFER, framebuffer);
      CheckGLError("glBindFramebuffer");
#endif

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
      if (TheVertexShader == NULL) {
          TheVertexShader = new VtxRender();
      }

      size.x = width;
      size.y = height;
      
      if (target == TargetTexture) {
        struct Tex *tex = TheTextureMgr->CreateTexture(width, height);
        
        texture = tex->id;

#ifndef OPENGL_11
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);
        CheckGLError("glFramebufferTexture2DOES");
#else
        glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D_OES, tex->id, 0);
        CheckGLError("glFramebufferTexture2D");
#endif
#ifdef TARGET_IOS
      } else {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        CheckGLError("glFramebufferRenderbuffer");
#endif
      }
      
#ifndef OPENGL_11
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      CheckGLError("glCheckFramebufferStatus");
      if (status != GL_FRAMEBUFFER_COMPLETE) {
#else
      GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
      CheckGLError("glCheckFramebufferStatus");
      if (status != GL_FRAMEBUFFER_COMPLETE_OES) {
#endif
        machete::common::Log("Frame buffer is not ready");
        return;
      }
      
      base = Mat4();
      base = base.Translate(-this->size.x / 2, this->size.y / 2, 0);
    }
    
    void DrawContext::Use() {
      lastTexBind = 0;
      
#ifndef OPENGL_11
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      CheckGLError("glBindFramebuffer");
#else
      glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
      CheckGLError("glBindFramebuffer");
#endif

      glViewport(0, 0, size.x, size.y);
      CheckGLError("glViewport");

#ifdef OPENGL_11
      glMatrixMode(GL_MODELVIEW);
      CheckGLError("glMatrixMode GL_MODELVIEW");
      glLoadIdentity();
      CheckGLError("glLoadIdentity");
      glTranslatef(0, size.y, 0);
      CheckGLError("glTranslatef");
#endif

      glClearColor(color.x, color.y, color.z, color.w);
      CheckGLError("glClearColor");
    }
    
    void DrawContext::Unuse() {
      if (renderer != NULL) {
        renderer->Unuse();
      }
      
#ifndef OPENGL_11
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      CheckGLError("glBindFramebuffer");
#else
      glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
      CheckGLError("glBindFramebuffer");
#endif
      
      renderer = NULL;
    }
    
    void DrawContext::StartFrame() {
      renderer = NULL;
      
      glClear(GL_COLOR_BUFFER_BIT);
      CheckGLError("glClear");
      lastTexBind = 0;
      
      NextBuffers();
    }
    
    void DrawContext::ChangeModelView(const machete::math::Mat4 & mv) {
      this->mv = mv;
      
      if (renderer != NULL) {
        renderer->SetModelView(mv);
      }
    }
    
    void DrawContext::Draw(Program *program, Vtx *verts, int vcount, unsigned short* elems, int ecount, const machete::math::Vec4 & tint, GLuint texId) {
      
      CheckNewProgram(program);
      
#ifdef OPENGL_11
      if (texId != lastTexBind || vtxCount + vcount > MAX_VTX || idxCount + ecount > MAX_IDX) {
        Draw();
      }
#else
      if (texId != lastTexBind || vtxCount + vcount > MAX_VTX || idxCount + ecount > MAX_IDX ||
          this->tint.x != tint.x || this->tint.y != tint.y || this->tint.z != tint.z || this->tint.w != tint.w) {
        Draw();
      }
#endif
      
      if (renderer == NULL) {
        return;
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
      
      if (renderer != NULL) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        CheckGLError("glBindBuffer ELEMENT");
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        CheckGLError("glBindBuffer ARRAY");
      
        glActiveTexture(GL_TEXTURE0);
        CheckGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, lastTexBind);
        CheckGLError("glBindTexture");

        renderer->Upload(vertexes, vtxCount, indices, idxCount, tint);
      }
      
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
      
      if (renderer != NULL) {
        renderer->Unuse();
      }
      
      renderer = NULL;
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
    
    void DrawContext::CheckNewProgram(machete::graphics::Program *program) {
      if (program == renderer) {
        return;
      }
      
      if (renderer != NULL) {
        Draw();
        renderer->Unuse();
      }
      
      renderer = program;
      
      renderer->Use();
      renderer->SetBase(base);
      renderer->ApplyOrtho(size.x, size.y);
      renderer->SetModelView(mv);      
      
      glViewport(0, 0, size.x, size.y);
      CheckGLError("glViewport");
    }
    
  }
}

