//
//  shader.h
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
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
      Program() { program = 0; projectionUni = 0; }
      ~Program();
      
      void Use() { glUseProgram(program); };
      void Unuse() { glUseProgram(0); };
      
      void ApplyOrtho(int width, int height);
      
    protected:
      bool CreateProgram(Shader *vtx, Shader *frag);
      
      GLuint program;
      GLuint projectionUni;
      
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
      GLuint id;
      GLuint width;
      GLuint height;
    };
    
    class TextureMgr {
    public:
      ~TextureMgr();
      Tex LoadTexture(const char *name);
      Tex CreateTexture(int width, int height);
      
    protected:
      machete::data::Hash<machete::data::Str, Tex> textures;
    };
    
    struct Vtx {
      machete::math::Vec4 pivot;
      machete::math::Vec4 vert;
      machete::math::Vec2 uv;
      machete::math::Vec2 scale;
      machete::math::Vec4 color;
      float rotation;
    };
    
    class VtxRender : public Program {
    public:
      VtxRender();
      ~VtxRender();
      void Upload(Vtx *verts, int vcount, unsigned short* elems, int ecount);
      
      void SetBase(const machete::math::Mat4 & base) { this->base = base; glUniformMatrix4fv(baseSlot, 1, 0, base.Pointer()); }
      void SetModelView(const machete::math::Mat4 & modelView) { this->modelView = modelView; glUniformMatrix4fv(modelviewSlot, 1, 0, modelView.Pointer()); }
      
      
    protected:
      GLuint pivotSlot;
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
