//
//  shader.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "shader.h"
#include "log.h"

#include "vtx.frag"
#include "vtx.vert"

using namespace machete::common;

namespace machete {
  namespace graphics {
    
    Shader::~Shader() {
      if (shader != 0) {
        glDeleteShader(shader);
      }
    }
    
    bool Shader::LoadShader(GLenum shaderType, const char *source) {
      
      shader = glCreateShader(shaderType);
      glShaderSource(shader, 1, &source, 0);
      glCompileShader(shader);
      
      GLint compileSuccess;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
      
      if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shader, sizeof(messages), 0, &messages[0]);
        
        Log(messages);
        exit(1);
        
        return false;
      }
      
      return true;
    }
    
    Program::~Program() {
      if (program != 0) {
        glDeleteProgram(program);
      }
    }
    
    bool Program::CreateProgram(Shader *vtx, Shader *frag) {
      program = glCreateProgram();
      glAttachShader(program, vtx->GetHandle());
      glAttachShader(program, frag->GetHandle());
      glLinkProgram(program);
      
      GLint linkSuccess;
      glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
      
      if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(program, sizeof(messages), 0, &messages[0]);
        
        Log(messages);
        
        exit(1);
        
        return false;
      }
      
      glUseProgram(program);

      projectionSlot = glGetUniformLocation(program, "Projection");
      
      glDisable(GL_DITHER);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glUseProgram(0);
      
      return true;
    }
    
    void Program::ApplyOrtho(int width, int height) {
      float ortho[16] = {
        2.0f / (float)width, 0, 0, 0,
        0, 2.0f / (float)height, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
      };
      
      glUniformMatrix4fv(projectionSlot, 1, 0, &ortho[0]);
    }
    
    ShaderMgr::ShaderMgr() {
      
    }
    
    ShaderMgr::~ShaderMgr() {
      // TODO: Destruir Shaders
    }
    
    Shader *ShaderMgr::LoadShader(GLenum shaderType, const char *name, const char *source) {
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
    }
    
    TextureMgr::~TextureMgr() {
      // TODO: Destruir Texturas
    }
    
    struct Tex* TextureMgr::LoadTexture(const char *name) {
      machete::data::Tree<machete::data::Str, struct Tex*> *txNode = textures.Seek(name);
      
      struct Tex *tex = new struct Tex();
      GLuint texId;
      
      if (txNode == NULL) {
        glGenTextures(1, &texId);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        
        void* pixels = NULL;
        machete::math::IVec2 size;
        
        machete::platform::ThePlatform->LoadImage(name, &pixels, size);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        
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
      
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texId);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      
      t->id = texId;
      
      return t;
    }
    
    VtxRender::VtxRender() {
      Shader *vtx = CreateVtxShader();
      Shader *frg = CreateFrgShader();
      
      CreateProgram(vtx, frg);
      
      pivotSlot = glGetAttribLocation(program, "Pivot");
      offsetSlot = glGetAttribLocation(program, "Offset");
      positionSlot = glGetAttribLocation(program, "Position");
      textureSlot = glGetAttribLocation(program, "TextureCoord");
      colorSlot = glGetAttribLocation(program, "SourceColor");
      scaleSlot = glGetAttribLocation(program, "Scale");
      rotationSlot = glGetAttribLocation(program, "Rotation");
      
      baseSlot = glGetUniformLocation(program, "Base");
      modelviewSlot = glGetUniformLocation(program, "Modelview");
      
      samplerSlot = glGetUniformLocation(program, "Sampler");
      
      glUniform1i(samplerSlot, 0);
    }
    
    Shader *VtxRender::CreateVtxShader() {
      return TheShaderMgr->LoadShader(GL_VERTEX_SHADER, "vtxVertex", VtxVertexShader);
    }
    
    Shader *VtxRender::CreateFrgShader() {
      return TheShaderMgr->LoadShader(GL_FRAGMENT_SHADER, "vtxFragment", VtxFragmentShader);
    }

    VtxRender::~VtxRender() {
      
    }
    
    void VtxRender::Use() {
      Program::Use();
      
      glEnableVertexAttribArray(pivotSlot);
      glEnableVertexAttribArray(offsetSlot);
      glEnableVertexAttribArray(positionSlot);
      glEnableVertexAttribArray(textureSlot);
      glEnableVertexAttribArray(colorSlot);
      glEnableVertexAttribArray(scaleSlot);
      glEnableVertexAttribArray(rotationSlot);
      glEnableVertexAttribArray(projectionSlot);
    }
    
    void VtxRender::Unuse() {
      glDisableVertexAttribArray(pivotSlot);
      glDisableVertexAttribArray(offsetSlot);
      glDisableVertexAttribArray(positionSlot);
      glDisableVertexAttribArray(textureSlot);
      glDisableVertexAttribArray(colorSlot);
      glDisableVertexAttribArray(scaleSlot);
      glDisableVertexAttribArray(rotationSlot);

      Program::Unuse();
    }
    
    void VtxRender::Upload(Vtx *verts, int vcount, unsigned short *elems, int ecount) {
      
      using namespace machete::data;
      
      glUniform1i(samplerSlot, 0);
      
      Vtx* POSITION = NULL;
      
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct Vtx) * vcount, verts);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short) * ecount, elems);
      
      glVertexAttribPointer(pivotSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].pivot);
      glVertexAttribPointer(offsetSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].offset);
      glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), POSITION);
      glVertexAttribPointer(textureSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].uv);
      glVertexAttribPointer(scaleSlot, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].scale);
      glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].color);
      glVertexAttribPointer(rotationSlot, 1, GL_FLOAT, GL_FALSE, sizeof(struct Vtx), &POSITION[0].rotation);
      
      glDrawElements(GL_TRIANGLES, ecount, GL_UNSIGNED_SHORT, POSITION);
    }
    
    ShaderMgr* TheShaderMgr = NULL;
    TextureMgr* TheTextureMgr = NULL;

  }
}
