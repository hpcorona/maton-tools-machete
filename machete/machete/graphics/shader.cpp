//
//  shader.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "shader.h"
#include "../common/log.h"

#ifndef OPENGL_11
#include "vtx.frag"
#include "vtx.vert"
#endif

using namespace machete::common;

namespace machete {
  namespace graphics {
    
    Shader::~Shader() {
#ifndef OPENGL_11
      if (shader != 0) {
        glDeleteShader(shader);
        CheckGLError("glDeleteShader");
      }
#endif
    }
    
    bool Shader::LoadShader(GLenum shaderType, const char *source) {
#ifndef OPENGL_11
      shader = glCreateShader(shaderType);
      CheckGLError("glCreateShader");
      glShaderSource(shader, 1, &source, 0);
      CheckGLError("glShaderSource");
      glCompileShader(shader);
      CheckGLError("glCompileShader");
      
      GLint compileSuccess;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
      CheckGLError("glGetShaderiv");
      
      if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shader, sizeof(messages), 0, &messages[0]);
        CheckGLError("glGetShaderInfoLog");
        
        Log(messages);
        
        return false;
      }
#endif
      
      return true;
    }
    
    Program::~Program() {
#ifndef OPENGL_11
      if (program != 0) {
        glDeleteProgram(program);
        CheckGLError("glDeleteProgram");
      }
#endif
    }
    
    void Program::Use() {
#ifndef OPENGL_11
      glUseProgram(program);
#endif
    }

    void Program::Unuse() {
#ifndef OPENGL_11
      glUseProgram(0);
#endif
    }

    bool Program::CreateProgram(Shader *vtx, Shader *frag) {
#ifndef OPENGL_11
      program = glCreateProgram();
      CheckGLError("glCreateProgram");
      glAttachShader(program, vtx->GetHandle());
      CheckGLError("glAttachShader");
      glAttachShader(program, frag->GetHandle());
      CheckGLError("glAttachShader");
      glLinkProgram(program);
      CheckGLError("glLinkProgram");
      
      GLint linkSuccess;
      glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
      CheckGLError("glGetProgramiv");
      
      if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(program, sizeof(messages), 0, &messages[0]);
        CheckGLError("glGetProgramInfoLog");
        
        Log(messages);
        
        return false;
      }
      
      glUseProgram(program);
      CheckGLError("glUseProgram");

      projectionSlot = glGetUniformLocation(program, "Projection");
      CheckGLError("glGetUniformLocation");
      
      glDisable(GL_DITHER);
      CheckGLError("glDisable DITHER");
      glDisable(GL_DEPTH_TEST);
      CheckGLError("glDisable DEPTH TEST");
      glEnable(GL_BLEND);
      CheckGLError("glDisable BLEND");
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      CheckGLError("glDisable SRC");

      glUseProgram(0);
      CheckGLError("glUseProgram");
#else
      glDisable(GL_DITHER);
      CheckGLError("glDisable DITHER");
      glDisable(GL_DEPTH_TEST);
      CheckGLError("glDisable DEPTH TEST");
      glEnable(GL_BLEND);
      CheckGLError("glDisable BLEND");
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      CheckGLError("glDisable SRC");
#endif
      
      return true;
    }
    
    void Program::ApplyOrtho(int width, int height) {
#ifndef OPENGL_11
      float ortho[16] = {
        2.0f / (float)width, 0, 0, 0,
        0, 2.0f / (float)height, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
      };

      glUniformMatrix4fv(projectionSlot, 1, 0, &ortho[0]);
      CheckGLError("glUniformMatrix4fv");
#else
      glViewport(0, 0, width, height);
      glMatrixMode(GL_PROJECTION);
      glOrthof(0, width, 0, height, -1, 1);
#endif
    }
    
    ShaderMgr::ShaderMgr() {
      
    }
    
    ShaderMgr::~ShaderMgr() {
      // TODO: Destruir Shaders
    }
    
    Shader *ShaderMgr::LoadShader(GLenum shaderType, const char *name, const char *source) {
#ifndef OPENGL_11
      Shader *shdr = NULL;
      
      machete::data::Tree<machete::data::Str, Shader*> *sdrNode = shaders.Seek(name);
      
      if (sdrNode == NULL) {
        shdr = new Shader();
        
        shdr->LoadShader(shaderType, source);
        
        shaders.Add(name, shdr);
      } else {
        shdr = sdrNode->GetValue();
      }
      
      return shdr;
#else
      return NULL;
#endif
    }
    
    TextureMgr::~TextureMgr() {
      machete::data::Iterator< machete::data::Tree<machete::data::Str, struct Tex*>* >* iter = textures.Enumerate();
      
      iter->Reset();
      
      while (iter->Next()) {
        struct Tex *tex = iter->GetCurrent()->GetValue()->GetValue();
        
        glDeleteTextures(1, &tex->id);
        CheckGLError("glDeleteTextures");
      }
      
      delete iter;
    }
    
    struct Tex* TextureMgr::LoadTexture(const char *name) {
      machete::data::Tree<machete::data::Str, struct Tex*> *txNode = textures.Seek(name);
      
      struct Tex *tex = NULL;
      GLuint texId;
      
      if (txNode == NULL) {
        tex = new struct Tex();
        
        glGenTextures(1, &texId);
        CheckGLError("glGenTextures");
        
        glActiveTexture(GL_TEXTURE0);
        CheckGLError("glActiveTexture");
        glBindTexture(GL_TEXTURE_2D, texId);
        CheckGLError("glBindTexture");
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);
        CheckGLError("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
        CheckGLError("glTexParameteri");
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        CheckGLError("glTexParameteri");
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        CheckGLError("glTexParameteri");
        
        void* pixels = NULL;
        machete::math::IVec2 size;
        
        machete::platform::ThePlatform->LoadImage(name, &pixels, size);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        CheckGLError("glTexImage2D");
        
        machete::platform::ThePlatform->UnloadImage();
        
        tex->id = texId;
        tex->width = size.x;
        tex->height = size.y;
        
        textures.Add(name, tex);
      } else {
        tex = txNode->GetValue();
      }
      
      return tex;
    }
    
    struct Tex* TextureMgr::CreateTexture(int width, int height) {
      struct Tex *t = new struct Tex();
      t->width = width;
      t->height = height;
      
      GLuint texId;
      
      glGenTextures(1, &texId);
      CheckGLError("glGenTextures");
      
      glActiveTexture(GL_TEXTURE0);
      CheckGLError("glActiveTexture");
      glBindTexture(GL_TEXTURE_2D, texId);
      CheckGLError("glBindTexture");

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR);
      CheckGLError("glTexParameteri");
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR);
      CheckGLError("glTexParameteri");
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
      CheckGLError("glTexParameteri");
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);
      CheckGLError("glTexParameteri");
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      CheckGLError("glTexImage2D");
      
      t->id = texId;
      
      return t;
    }
    
    VtxRender::VtxRender() {
#ifndef OPENGL_11
      Shader *vtx = CreateVtxShader();
      Shader *frg = CreateFrgShader();
      
      CreateProgram(vtx, frg);
      
      pivotSlot = glGetAttribLocation(program, "Pivot");
      CheckGLError("glGetAttribLocation");
      offsetSlot = glGetAttribLocation(program, "Offset");
      CheckGLError("glGetAttribLocation");
      positionSlot = glGetAttribLocation(program, "Position");
      CheckGLError("glGetAttribLocation");
      textureSlot = glGetAttribLocation(program, "TextureCoord");
      CheckGLError("glGetAttribLocation");
      scaleSlot = glGetAttribLocation(program, "Scale");
      CheckGLError("glGetAttribLocation");
      rotationSlot = glGetAttribLocation(program, "Rotation");
      CheckGLError("glGetAttribLocation");
      
      baseSlot = glGetUniformLocation(program, "Base");
      CheckGLError("glGetUniformLocation");
      modelviewSlot = glGetUniformLocation(program, "Modelview");
      CheckGLError("glGetUniformLocation");
      destColorSlot = glGetUniformLocation(program, "DestinationColor");
      CheckGLError("glGetUniformLocation");
      
      samplerSlot = glGetUniformLocation(program, "Sampler");
      CheckGLError("glGetUniformLocation");
#else
      // TODO: OpenGL ES 1.1 Renderer Initialization
#endif
    }
    
    Shader *VtxRender::CreateVtxShader() {
#ifndef OPENGL_11
      return TheShaderMgr->LoadShader(GL_VERTEX_SHADER, "vtxVertex", VtxVertexShader);
#else
      return NULL;
#endif
    }
    
    Shader *VtxRender::CreateFrgShader() {
#ifndef OPENGL_11
      return TheShaderMgr->LoadShader(GL_FRAGMENT_SHADER, "vtxFragment", VtxFragmentShader);
#else
      return NULL;
#endif
    }

    VtxRender::~VtxRender() {
      
    }
    
    void VtxRender::Use() {
      Program::Use();
      
#ifndef OPENGL_11
      glEnableVertexAttribArray(pivotSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(offsetSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(positionSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(textureSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(scaleSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(rotationSlot);
      CheckGLError("glEnableVertexAttribArray");
      glEnableVertexAttribArray(projectionSlot);
      CheckGLError("glEnableVertexAttribArray");
#else
      glEnableClientState(GL_VERTEX_ARRAY);
      CheckGLError("glEnableClientState GL_VERTEX_ARRAY");
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      CheckGLError("glEnableClientState GL_TEXCURE_COORD_ARRAY");
      glEnableClientState(GL_COLOR_ARRAY);
      CheckGLError("glEnableClientState GL_COLOR_ARRAY");
#endif
    }
    
    void VtxRender::Unuse() {
#ifndef OPENGL_11
      glDisableVertexAttribArray(pivotSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(offsetSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(positionSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(textureSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(scaleSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(rotationSlot);
      CheckGLError("glDisableVertexAttribArray");
      glDisableVertexAttribArray(projectionSlot);
      CheckGLError("glDisableVertexAttribArray");
#else
      glDisableClientState(GL_VERTEX_ARRAY);
      CheckGLError("glDisableClientState GL_VERTEX_ARRAY");
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      CheckGLError("glDisableClientState GL_TEXTURE_COORD_ARRAY");
      glDisableClientState(GL_COLOR_ARRAY);
      CheckGLError("glDisableClientState GL_COLOR_ARRAY");
#endif

      Program::Unuse();
    }
    
    void VtxRender::Upload(Vtx *verts, int vcount, unsigned short *elems, int ecount, const machete::math::Vec4 & color) {
      using namespace machete::data;

      Vtx* POSITION = NULL;

      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct Vtx) * vcount, verts);
      CheckGLError("glBufferSubData");
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short) * ecount, elems);
      CheckGLError("glBufferSubData");

#ifndef OPENGL_11
      glUniform4f(destColorSlot, color.x, color.y, color.z, color.w);
      CheckGLError("glUniform4f");
      glUniform1i(samplerSlot, 0);
      CheckGLError("glUniform1i");
      
      glVertexAttribPointer(pivotSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].pivot);
      CheckGLError("glVertexAttribPointer PIVOT");
      glVertexAttribPointer(offsetSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].offset);
      CheckGLError("glVertexAttribPointer OFFSET");
      glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), POSITION);
      CheckGLError("glVertexAttribPointer POSITION");
      glVertexAttribPointer(textureSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].uv);
      CheckGLError("glVertexAttribPointer UV");
      glVertexAttribPointer(scaleSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].scale);
      CheckGLError("glVertexAttribPointer SCALE");
      glVertexAttribPointer(rotationSlot, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].rotation);
      CheckGLError("glVertexAttribPointer ROTATION");
#else
      glVertexPointer(3, GL_FLOAT, sizeof(struct Vtx), POSITION);
      CheckGLError("glVertexPointer");
      glTexCoordPointer(2, GL_FLOAT, sizeof(struct Vtx), &POSITION[0].uv);
      CheckGLError("glTexCoordPointer");
      glColorPointer(4, GL_FLOAT, sizeof(struct Vtx), &POSITION[0].color);
      CheckGLError("glColorPointer");
#endif

      glDrawElements(GL_TRIANGLES, ecount, GL_UNSIGNED_SHORT, POSITION);
      CheckGLError("glDrawElements");
    }
    
    void VtxRender::SetBase(const machete::math::Mat4 & base) {
      this->base = base;
#ifndef OPENGL_11
      glUniformMatrix4fv(baseSlot, 1, 0, base.Pointer());
#endif
    }

    void VtxRender::SetModelView(const machete::math::Mat4 & modelView) {
      this->modelView = modelView;
#ifndef OPENGL_11
      glUniformMatrix4fv(modelviewSlot, 1, 0, modelView.Pointer());
#endif
    }

    ShaderMgr* TheShaderMgr = NULL;
    TextureMgr* TheTextureMgr = NULL;
    VtxRender* TheVertexShader = NULL;

  }
}
