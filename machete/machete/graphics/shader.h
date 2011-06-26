//
//  shader.h
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#define STRINGIFY(A) #A

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "../data/str.h"
#include "../data/tree.h"
#include "../math/vector.h"
#include "../math/matrix.h"
#include "../platform/platform.h"

namespace machete {
  namespace graphics {
    
    static void CheckGLError() {
      GLenum status = glGetError();
      
      switch (status) {
        case 1280:
          machete::common::Log("1280 GL_INVALID_ENUM");
          break;
        case 1281:
          machete::common::Log("1281 GL_INVALID_VALUE");
          break;
        case 1282:
          machete::common::Log("1282 GL_INVALID_OPERATION");
          break;
        case 1283:
          machete::common::Log("1283 GL_STACK_OVERFLOW");
          break;
        case 1284:
          machete::common::Log("1284 GL_STACK_UNDERFLOW");
          break;
        case 1285:
          machete::common::Log("1285 GL_OUT_OF_MEMORY");
          break;
      }
    }
    
    class Shader {
    public:
      Shader() { shader = 0; }
      ~Shader();
      
      GLuint GetHandle() const { return shader; }
      bool LoadShader(GLenum shaderType, const char* source);
      
    protected:
      GLuint shader;
    };
    
    class Program {
    public:
      Program() { program = 0; projectionSlot = 0; }
      ~Program();
      
      void Use() { glUseProgram(program); };
      void Unuse() { glUseProgram(0); };
      
      void ApplyOrtho(int width, int height);
      
    protected:
      bool CreateProgram(Shader *vtx, Shader *frag);
      
      GLuint program;
      GLuint projectionSlot;
      
      Shader *vertexShader;
      Shader *fragmentShader;
    };
    
    class ShaderMgr {
    public:
      ShaderMgr();
      ~ShaderMgr();
      Shader *LoadShader(GLenum shaderType, const char *name, const char *source);
      
    protected:
      machete::data::Hash<machete::data::Str, Shader*> shaders;
    };
    
    struct Tex {
      unsigned int id;
      float width;
      float height;
    };
    
    class TextureMgr {
    public:
      ~TextureMgr();
      struct Tex* LoadTexture(const char *name);
      struct Tex* CreateTexture(int width, int height);
      
    protected:
      machete::data::Hash<machete::data::Str, struct Tex*> textures;
    };
    
    struct Vtx {
      Vtx() : color(1, 1, 1, 1), scale(1, 1) { rotation = 0; }
      
      machete::math::Vec3 vert;
      machete::math::Vec2 offset;
      machete::math::Vec2 pivot;
      machete::math::Vec2 uv;
      machete::math::Vec2 scale;
      machete::math::Vec4 color;
      float rotation;
    };
    
    
    class VtxRender : public Program {
    public:
      VtxRender();
      ~VtxRender();
      Shader *CreateVtxShader();
      Shader *CreateFrgShader();
      
      void Use();
      void Unuse();
      
      void Upload(Vtx *verts, int vcount, unsigned short* elems, int ecount);
      
      void SetBase(const machete::math::Mat4 & base) { this->base = base; glUniformMatrix4fv(baseSlot, 1, 0, base.Pointer()); }
      void SetModelView(const machete::math::Mat4 & modelView) { this->modelView = modelView; glUniformMatrix4fv(modelviewSlot, 1, 0, modelView.Pointer()); }
      
      
    protected:
      GLuint pivotSlot;
      GLuint offsetSlot;
      GLuint positionSlot;
      GLuint textureSlot;
      GLuint colorSlot;
      GLuint scaleSlot;
      GLuint rotationSlot;
      
      GLuint baseSlot;
      GLuint modelviewSlot;
      
      GLuint samplerSlot;
      
      machete::math::Mat4 base;
      machete::math::Mat4 modelView;
    };
    
    
    extern ShaderMgr* TheShaderMgr;
    extern TextureMgr* TheTextureMgr;
    
  }
}
