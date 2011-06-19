//
//  shader.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
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

      projectionUni = glGetUniformLocation(program, "Projection");

      return true;
    }
    
    void Program::ApplyOrtho(int width, int height) {
      float ortho[16] = {
        2.0f / (float)width, 0, 0, 0,
        0, 2.0f / (float)height, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
      };
      
      glUniformMatrix4fv(projectionUni, 1, 0, &ortho[0]);
    }
    
    ShaderMgr::ShaderMgr() {
      glDisable(GL_DITHER);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    
    Tex TextureMgr::LoadTexture(const char *name) {
      machete::data::Tree<machete::data::Str, Tex> *txNode = textures.Seek(name);
      
      Tex tex;
      
      if (txNode == NULL) {
        glGenTextures(1, &tex.id);
        glBindTexture(GL_TEXTURE_2D, tex.id);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        
        char* pixels = NULL;
        machete::math::IVec2 size;
        
        machete::platform::ThePlatform->LoadImage(name, &pixels, size);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        
        delete pixels;
        
        tex.width = size.x;
        tex.height = size.y;
        
        textures.Add(name, tex);
      } else {
        tex = txNode->GetValue();
      }
      
      return tex;
    }
    
    Tex TextureMgr::CreateTexture(int width, int height) {
      Tex t;
      t.width = width;
      t.height = height;
      
      glGenTextures(1, &t.id);
      
      glBindTexture(GL_TEXTURE_2D, t.id);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      GL_CLAMP_TO_EDGE);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      
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
      rotationSlot = glGetAttribLocation(program, "Sampler");
      
      baseSlot = glGetAttribLocation(program, "Base");
      modelviewSlot = glGetAttribLocation(program, "Modelview");
      
      samplerSlot = glGetAttribLocation(program, "Sampler");
      
      glEnableVertexAttribArray(pivotSlot);
      glEnableVertexAttribArray(offsetSlot);
      glEnableVertexAttribArray(positionSlot);
      glEnableVertexAttribArray(textureSlot);
      glEnableVertexAttribArray(colorSlot);
      glEnableVertexAttribArray(scaleSlot);
      glEnableVertexAttribArray(rotationSlot);
      
      glUniform1f(samplerSlot, 0);
    }
    
    Shader *VtxRender::CreateVtxShader() {
      return TheShaderMgr->LoadShader(GL_VERTEX_SHADER, "vtxVertex", VtxVertexShader);
    }
    
    Shader *VtxRender::CreateFrgShader() {
      return TheShaderMgr->LoadShader(GL_FRAGMENT_SHADER, "vtxFragment", VtxFragmentShader);
    }

    VtxRender::~VtxRender() {
      
    }
    
    void VtxRender::Upload(Vtx *verts, int vcount, unsigned short *elems, int ecount) {
      
      Vtx* POSITION = NULL;
      
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vtx) * vcount, verts);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * ecount, elems);
      
      glVertexAttribPointer(pivotSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].pivot);
      glVertexAttribPointer(offsetSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].offset);
      glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vtx), POSITION);
      glVertexAttribPointer(textureSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].uv);
      glVertexAttribPointer(scaleSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].scale);
      glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].color);
      glVertexAttribPointer(rotationSlot, 1, GL_FLOAT, GL_FALSE, sizeof(Vtx), &POSITION[0].rotation);
      
      glDrawElements(GL_TRIANGLES, vcount, GL_UNSIGNED_SHORT, POSITION);
    }
    
    ShaderMgr* TheShaderMgr = NULL;
    TextureMgr* TheTextureMgr = NULL;

  }
}
