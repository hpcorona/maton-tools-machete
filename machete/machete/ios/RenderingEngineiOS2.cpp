//
//  RenderingEngineiOS2.cpp
//  machete
//
//  Created by Hilario Perez Corona on 4/10/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "RenderingEngineiOS2.h"

#ifdef TARGET_IOS

namespace machete {
  
  RenderingEngineiOS2::RenderingEngineiOS2(RenderTarget t, IResourceManager *rm) {
    target = t;
    resMan = rm;
  }
  
  ivec2 RenderingEngineiOS2::GetScreenSize() const { return size; }
  
  void RenderingEngineiOS2::Initialize(int width, int height) {
    if (target == TargetTexture) {
      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_OES, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    } else {
      glGenRenderbuffers(1, &renderbuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

      glGenFramebuffers(1, &framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    }
    
    vtxBind = -1;
    lastBind = -1;
    size.x = width;
    size.y = height;
    
    glDisable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);
    
    simpleProgram = BuildProgram(SimpleVertexShader, SimpleFragmentShader);
    glUseProgram(simpleProgram);
    
    positionSlot = glGetAttribLocation(simpleProgram, "Position");
    textureSlot = glGetAttribLocation(simpleProgram, "TextureCoord");
    colorSlot = glGetAttribLocation(simpleProgram, "SourceColor");
    midRotScaleSlot = glGetAttribLocation(simpleProgram, "MidRotScale");
    
    samplerSlot = glGetUniformLocation(simpleProgram, "Sampler");
    modelviewSlot = glGetUniformLocation(simpleProgram, "Modelview");
    
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(colorSlot);
    glEnableVertexAttribArray(textureSlot);
    glEnableVertexAttribArray(midRotScaleSlot);
    
    glUniform1f(samplerSlot, 0);
    
    ApplyOrtho(width, height);
    
    base = mat4();
    base = base.Translate(-this->size.x / 2, this->size.y / 2, 0);
    glUniformMatrix4fv(modelviewSlot, 1, 0, base.Pointer());
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    
    CreateBuffers();
  }
  
  void RenderingEngineiOS2::Clear() {
    glBindFramebuffer(1, framebuffer);
    if (target == TargetTexture) {
      glBindTexture(GL_TEXTURE_2D, tex);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_OES, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    } else {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    }
    
    glBindTexture(GL_TEXTURE_2D, lastBind);
    glActiveTexture(GL_TEXTURE0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    primCount = 0;
    vtxCount = 0;
  }
  
  void RenderingEngineiOS2::Draw(vec2 pos, vec2 size, float rotation, float scale, vertuv *verts, unsigned int texId) {
    if (lastBind != texId || vtxCount == MAX_IDX) {
      Draw();
      
      if (lastBind != texId) {
        glBindTexture(GL_TEXTURE_2D, texId);
        glActiveTexture(GL_TEXTURE0);
        lastBind = texId;
      }
    }
    
    vertexes[primCount++] = verts[0];
    vertexes[primCount++] = verts[1];
    vertexes[primCount++] = verts[2];
    vertexes[primCount++] = verts[3];
    
    vtxCount += 6;
  }
  
  void RenderingEngineiOS2::Draw() {
    if (vtxCount == 0) return;
    
    vertuv* POSITION = NULL;
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertuv) * primCount, vertexes);
    
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(vertuv), POSITION);
    glVertexAttribPointer(textureSlot, 4, GL_FLOAT, GL_FALSE, sizeof(vertuv), &POSITION[0].uv);
    glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(vertuv), &POSITION[0].color);
    glVertexAttribPointer(midRotScaleSlot, 4, GL_FLOAT, GL_FALSE, sizeof(vertuv), &POSITION[0].mra);
    
    glDrawElements(GL_TRIANGLES, vtxCount, GL_UNSIGNED_SHORT, POSITION);
    
    vtxCount = 0;
    primCount = 0;
    
    ringIdx++;
    if (ringIdx >= MAX_RING) {
      ringIdx = 0;
    }
    vertexes = vertexesRing[ringIdx];
    vertexBuffer = vertexBufferRing[ringIdx];
  }
  
  GLuint RenderingEngineiOS2::CreateBuffer(vertuv* verts) const {
    GLuint buff;
    
    glGenBuffers(1, &buff);
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertuv) * MAX_VTX, verts, GL_DYNAMIC_DRAW);
    
    return buff;
  }
  
  void RenderingEngineiOS2::DeleteBuffer(GLuint buffer) const {
    glDeleteBuffers(1, &buffer);
  }
  
  Tex RenderingEngineiOS2::CreateTexture(const char* texture) {
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
    
    resMan->LoadImage(texture);
    void* pixels = resMan->GetImageData();
    ivec2 size = resMan->GetImageSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    resMan->UnloadImage();
    
    return tTex;
  }
  
  void RenderingEngineiOS2::ApplyOrtho(float maxX, float maxY) const {
    float ortho[16] = {
      2.0f / maxX, 0, 0, 0,
      0, 2.0f / maxY, 0, 0,
      0, 0, -1, 0,
      0, 0, 0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(simpleProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
  }
  
  GLuint RenderingEngineiOS2::BuildProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const {
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
      std::cout << messages;
      exit(1);
    }
    
    return programHandle;
  }
  
  GLuint RenderingEngineiOS2::BuildShader(const char* source, GLenum shaderType) const {
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
      std::cout << messages;
      exit(1);
    }
    
    return shaderHandle;
  }
  
  void RenderingEngineiOS2::CreateBuffers() {
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
