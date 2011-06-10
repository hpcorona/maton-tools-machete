//
//  shader.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "shader.h"
#include "log.h"

namespace machete {
  namespace common {
    
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
    
    bool Program::CreateProgram(machete::common::Shader *vtx, machete::common::Shader *frag) {
      
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
      
      return true;
    }
    
    ShaderMgr::~ShaderMgr() {
      // TODO: Destruir Shaders
    }
    
    Shader *ShaderMgr::LoadShader(GLenum shaderType, const char *name, const char *source) {
      Shader *shdr = NULL;
      
      machete::data::Tree<machete::data::Str, Shader*> *sdrNode = shaders.Seek(name);
      
      if (shdr == NULL) {
        shdr = new Shader();
        
        shdr->LoadShader(shaderType, source);
        
        shaders.Add(name, shdr);
      } else {
        shdr = sdrNode->GetValue();
      }
      
      return shdr;
    }
    
  }
}
