//
//  shader.h
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "../str.h"
#include "../tree.h"

namespace machete {
  namespace common {
    
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
      Program() { program = 0; }
      ~Program();

    protected:
      bool CreateProgram(Shader *vtx, Shader *frag);

      GLuint program;
      
      Shader *vertexShader;
      Shader *fragmentShader;
    };
    
    class ShaderMgr {
    public:
      
      ~ShaderMgr();
      Shader *LoadShader(GLenum shaderType, const char *name, const char *source);
      
    protected:
      machete::data::Hash<machete::data::Str, Shader*> shaders;
    };
    
    static ShaderMgr TheShaderMgr;
    
  }
}
