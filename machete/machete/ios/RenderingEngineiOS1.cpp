//
//  RenderingEngineiOS1.cpp
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "RenderingEngineiOS1.h"

#ifdef TARGET_IOS

namespace machete {
  
  RenderingEngineiOS1::RenderingEngineiOS1(RenderTarget t, IResourceManager* sm) {
    resMan = sm;
    target = t;
    
    if (target == TargetScreen) {
      glGenRenderbuffersOES(1, &renderbuffer);
      glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
    }
  }
  
  ivec2 RenderingEngineiOS1::GetScreenSize() const { return size; }
  
  void RenderingEngineiOS1::Initialize(int width, int height) {
    if (target == TargetTexture) {
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8_OES, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
      glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, tex, 0);
    }
    
    lastBind = -1;
    vtxBind = -1;
    primCount = 0;
    vtxCount = 0;
    
    size.x = width;
    size.y = height;
    
    glDisable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glGenFramebuffersOES(1, &framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderbuffer);
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glOrthof(0, width, 0, height, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, size.y, 0);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    
    CreateBuffers();
  }
  
  void RenderingEngineiOS1::Clear() {
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT);
    primCount = 0;
    vtxCount = 0;
  }
  
  void RenderingEngineiOS1::Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv *verts, unsigned int texId) {
    
    if (lastBind != texId || vtxCount == MAX_IDX) {
      Draw();
      
      if (lastBind != texId) {
        glBindTexture(GL_TEXTURE_2D, texId);
        glActiveTexture(GL_TEXTURE0);
        lastBind = texId;
      }
    }
    
    if (rotation == 0.0f && scale == 1.0f) {
      vertexes[primCount++] = verts[0];
      vertexes[primCount++] = verts[1];
      vertexes[primCount++] = verts[2];
      vertexes[primCount++] = verts[3];
      
      vec4 pos4 = vec4(pos.x, pos.y, 0, 0);
      vertexes[primCount - 4].vert += pos4;
      vertexes[primCount - 3].vert += pos4;
      vertexes[primCount - 2].vert += pos4;
      vertexes[primCount - 1].vert += pos4;
    } else {
      mat4 loc;
      
      loc = loc.Translate(-size.x / 2, size.y / 2, 0) * loc.Scale(scale) * loc.Rotate(rotation) * loc.Translate(size.x / 2, -size.y / 2, 0) * loc.Translate(pos.x, pos.y, 0);
      
      mat4 v1 = loc.Translate(verts[0].vert) * loc;
      mat4 v2 = loc.Translate(verts[1].vert) * loc;
      mat4 v3 = loc.Translate(verts[2].vert) * loc;
      mat4 v4 = loc.Translate(verts[3].vert) * loc;
      
      
      vertexes[primCount].vert = v1.w; vertexes[primCount].uv = verts[0].uv; vertexes[primCount].color = verts[0].color;
      primCount++;
      vertexes[primCount].vert = v2.w; vertexes[primCount].uv = verts[1].uv; vertexes[primCount].color = verts[1].color;
      primCount++;
      vertexes[primCount].vert = v3.w; vertexes[primCount].uv = verts[2].uv; vertexes[primCount].color = verts[2].color;
      primCount++;
      vertexes[primCount].vert = v4.w; vertexes[primCount].uv = verts[3].uv; vertexes[primCount].color = verts[3].color;
      primCount++;
    }
    vtxCount += 6;
  }
  
  void RenderingEngineiOS1::Draw() {
    if (vtxCount == 0) return;
    
    vertuv* POSITION = 0;
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertuv) * primCount, vertexes);
    
    glVertexPointer(3, GL_FLOAT, sizeof(vertuv), &POSITION[0].vert);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertuv), &POSITION[0].uv);
    glColorPointer(4, GL_FLOAT, sizeof(vertuv), &POSITION[0].color);
    
    glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_SHORT, 0);
    
    vtxCount = 0;
    primCount = 0;
    
    ringIdx++;
    if (ringIdx >= MAX_RING) {
      ringIdx = 0;
    }
    vertexes = vertexesRing[ringIdx];
    vertexBuffer = vertexBufferRing[ringIdx];
  }
  
  GLuint RenderingEngineiOS1::CreateBuffer(vertuv* verts) const {
    GLuint buff;
    
    glGenBuffers(1, &buff);
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertuv) * MAX_VTX, verts, GL_DYNAMIC_DRAW);
    
    return buff;
  }
  
  void RenderingEngineiOS1::DeleteBuffer(GLuint buffer) const {
    glDeleteBuffers(1, &buffer);
  }
  
  
  Tex RenderingEngineiOS1::CreateTexture(const char* texture) {
    Tex tTex;
    
    glGenTextures(1, &tTex.id);
    glBindTexture(GL_TEXTURE_2D, tTex.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
    
    resMan->LoadImage(texture);
    void* pixels = resMan->GetImageData();
    ivec2 size = resMan->GetImageSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    resMan->UnloadImage();
    
    return tTex;
  }
  
  
  void RenderingEngineiOS1::CreateBuffers() {
    unsigned int vtx = 0;
    
    for (unsigned int idx = 0; idx < MAX_IDX; idx += 6) {
      indices[idx] = vtx; indices[idx+1] = vtx+1; indices[idx+2] = vtx+2;
      indices[idx+3] = vtx+2; indices[idx+4] = vtx+1; indices[idx+5] = vtx+3;
      
      vtx += 4;
    }
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MAX_IDX, indices, GL_STATIC_DRAW);
    
    for (unsigned int i = 0; i < MAX_RING; i++) {
      vertexBufferRing[i] = CreateBuffer(vertexesRing[i]);
    }
    
    ringIdx = 0;
    vertexes = vertexesRing[ringIdx];
    vertexBuffer = vertexBufferRing[ringIdx];
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    vtxCount = 0;
    primCount = 0;
  }
  
}

#endif
