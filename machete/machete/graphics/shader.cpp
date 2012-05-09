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
      if (this->source != NULL) {
        delete this->source;
      }
      this->source = new char[strlen(source)+1];
      this->source[strlen(source)] = 0;
      strcpy(this->source, source);
      
      this->shaderType = shaderType;
      
      return CreateShader();
#else
      return true;
#endif
    }
    
    void Shader::Regenerate() {
      CreateShader();
    }
    
    bool Shader::CreateShader() {
#ifndef OPENGL_11
      shader = glCreateShader(shaderType);
      CheckGLError("glCreateShader");
      glShaderSource(shader, 1, (const char**)&this->source, 0);
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
    
    void Program::Regenerate() {
#ifndef OPENGL_11
      CreateProgram(vertexShader, fragmentShader);
      
      Extract();
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
    
    void ShaderMgr::Regenerate() {
      machete::data::Iterator< machete::data::Tree<machete::data::Str, Shader*>* >* iter = shaders.Enumerate();
      
      iter->Reset();
      while (iter->Next()) {
        machete::data::Tree<machete::data::Str, Shader*>* node;
        node = iter->GetCurrent()->GetValue();
        
        node->GetValue()->Regenerate();
      }
    }
    
    TextureMgr::~TextureMgr() {
      machete::data::Iterator< machete::data::Tree<machete::data::Str, Texture*>* >* iter = textures.Enumerate();
      
      iter->Reset();
      
      while (iter->Next()) {
        Texture *tex = iter->GetCurrent()->GetValue()->GetValue();
        
        glDeleteTextures(1, &tex->id);
        CheckGLError("glDeleteTextures");
      }
      delete iter;
      
      dynTextures.Reset();
      while (dynTextures.Next()) {
        Texture *tex = dynTextures.GetCurrent()->GetValue();
        
        glDeleteTextures(1, &tex->id);
        CheckGLError("glDeleteTextures");
      }
      
      resources.RemoveAll();
    }
    
    void TextureMgr::Regenerate() {
      machete::data::Iterator< machete::data::Tree<machete::data::Str, Texture*>* >* iter = textures.Enumerate();
      
      iter->Reset();
      char fileName[200];
      machete::data::Str fileNameStr(200);
      
      while (iter->Next()) {
        machete::data::Tree<machete::data::Str, Texture*>* node;
        node = iter->GetCurrent()->GetValue();
        fileNameStr = node->GetKey();
        fileNameStr.GetChars(fileName, 200);
        Texture *tex = iter->GetCurrent()->GetValue()->GetValue();
        
        InternalLoad(fileName, tex);
      }
      delete iter;
      
      dynTextures.Reset();
      while (dynTextures.Next()) {
        Texture *tex = dynTextures.GetCurrent()->GetValue();
        
        InternalCreate((int)tex->width, (int)tex->height, tex);
      }
      
      resources.Reset();
      while (resources.Next()) {
        Regen *res = resources.GetCurrent()->GetValue();
        
        res->Regenerate();
      }
    }
    
    void TextureMgr::RegisterRegen(machete::graphics::Regen *reg) {
      resources.Append(reg);
    }
    
    void TextureMgr::UnregisterRegen(machete::graphics::Regen *reg) {
      resources.Reset();
      
      while (resources.Next()) {
        if (resources.GetCurrent()->GetValue() == reg) {
          resources.RemoveCurrent(true);
        }
      }
    }

    void TextureMgr::Unload(Texture *texture) {
      machete::data::Iterator< machete::data::Tree<machete::data::Str, Texture*>* >* iter = textures.Enumerate();
      
      iter->Reset();
      machete::data::Str fileNameStr(200);
      
      while (iter->Next()) {
        machete::data::Tree<machete::data::Str, Texture*>* node;
        if (node->GetValue() == texture) {
          glDeleteTextures(1, &node->GetValue()->id);
          CheckGLError("glDeleteTextures");
          textures.Delete(node->GetKey());
          
          delete iter;
          return;
        }
      }
      delete iter;
      
      dynTextures.Reset();
      while (dynTextures.Next()) {
        Texture *tex = dynTextures.GetCurrent()->GetValue();
        if (tex == texture) {
          glDeleteTextures(1, &tex->id);
          CheckGLError("glDeleteTextures");
          return;
        }
      }
    }
    
    Texture* TextureMgr::LoadTexture(const char *name) {
      machete::data::Tree<machete::data::Str, Texture*> *txNode = textures.Seek(name);
      
      Texture *tex = NULL;
      
      if (txNode == NULL) {
        tex = new Texture();
        
        InternalLoad(name, tex);
        
        textures.Add(name, tex);
      } else {
        tex = txNode->GetValue();
      }
      
      return tex;
    }
    
    Texture* TextureMgr::InternalLoad(const char *name, Texture* tex) {
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
      
      void* pixels = NULL;
      machete::math::IVec2 size;
      
      machete::platform::ThePlatform->LoadImage(name, &pixels, size);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      CheckGLError("glTexImage2D");
      
      machete::platform::ThePlatform->UnloadImage();
      
      tex->id = texId;
      tex->width = size.x;
      tex->height = size.y;
      
      return tex;
    }
    
    Texture* TextureMgr::CreateTexture(int width, int height) {
      Texture *t = new Texture();
      
      InternalCreate(width, height, t);
      
      dynTextures.Append(t);
      
      return t;
    }
    
    Texture* TextureMgr::InternalCreate(int width, int height, Texture* tex) {
      tex->width = width;
      tex->height = height;
      
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
      
      tex->id = texId;
      
      return tex;
    }
    
    VtxRender::VtxRender() {
#ifndef OPENGL_11
      vertexShader = CreateVtxShader();
      fragmentShader = CreateFrgShader();
      
      CreateProgram(vertexShader, fragmentShader);
      
      Extract();
#else
      // TODO: OpenGL ES 1.1 Renderer Initialization
#endif
    }
    
    void VtxRender::Extract() {
#ifndef OPENGL_11
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
      glUniformMatrix4fv(baseSlot, 1, 0, this->base.Pointer());
      CheckGLError("glUniformMatrix4fv");
#endif
    }

    void VtxRender::SetModelView(const machete::math::Mat4 & modelView) {
      this->modelView = modelView;
#ifndef OPENGL_11
      glUniformMatrix4fv(modelviewSlot, 1, 0, this->modelView.Pointer());
      CheckGLError("glUniformMatrix4fv");
#endif
    }

    ShaderMgr* TheShaderMgr = NULL;
    TextureMgr* TheTextureMgr = NULL;
    VtxRender* TheVertexShader = NULL;

  }
}
