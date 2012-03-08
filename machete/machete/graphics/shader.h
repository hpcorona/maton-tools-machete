//
//  shader.h
//  machete
//
//  Created by Hilario Perez Corona on 6/8/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file shader.h
//! \brief Low level drawing classes and structures.

#pragma once

#define STRINGIFY(A) #A

#ifdef TARGET_ANDROID
#ifdef OPENGL_11
#define GL_GLEXT_PROTOTYPES
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#elif TARGET_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#include "../data/str.h"
#include "../data/tree.h"
#include "../math/vector.h"
#include "../math/matrix.h"
#include "../platform/platform.h"

namespace machete {
  
  //! Low level graphics classes and structures.
  namespace graphics {
    
    //! Check for OpenGL errors and display the error on the console.
    static void CheckGLError(const char *func) {
#ifdef MACHETE_OPENGL_DEBUG
      GLenum status = glGetError();
      
      switch (status) {
        case 1280:
          machete::common::Log(func);
          machete::common::Log("1280 GL_INVALID_ENUM");
          break;
        case 1281:
          machete::common::Log(func);
          machete::common::Log("1281 GL_INVALID_VALUE");
          break;
        case 1282:
          machete::common::Log(func);
          machete::common::Log("1282 GL_INVALID_OPERATION");
          break;
        case 1283:
          machete::common::Log(func);
          machete::common::Log("1283 GL_STACK_OVERFLOW");
          break;
        case 1284:
          machete::common::Log(func);
          machete::common::Log("1284 GL_STACK_UNDERFLOW");
          break;
        case 1285:
          machete::common::Log(func);
          machete::common::Log("1285 GL_OUT_OF_MEMORY");
          break;
      }
#endif
    }
    
    //! Check for OpenGL errors and display the error on the console.
    static void CheckGLError() {
#ifdef MACHETE_OPENGL_DEBUG
      CheckGLError("");
#endif
    }
    
    //! Basic Shader functionality.
    class Shader {
    public:
      
      //! Creates a new empty shader.
      Shader() { shader = 0; }
      
      //! Destructor.
      ~Shader();
      
      //! Get the current shader handler.
      GLuint GetHandle() const { return shader; }
      
      //! Compile a piece of shader code. It can be a vertex or fragment shader.
      /*!
       \param shaderType The type of shader, as stated in OpenGL.
       \param source The source code of the shader.
       \return true if the shader was compiled successfully. false otherwise.
       */
      bool LoadShader(GLenum shaderType, const char* source);
      
    protected:
      
      //! The shader handler.
      GLuint shader;
    };
    
    //! Represents a Vertex to be Uploaded to the Graphics Hardware.
    struct Vtx {
      //! Creates a new Vertex.
#ifndef OPENGL_11
      Vtx() : scale(1, 1) { rotation = 0; }
#else
      Vtx() {}
#endif
      
      //! Vertex position.
      machete::math::Vec3 vert;
      
      //! Texture U/V for this vertex.
      machete::math::Vec2 uv;

#ifdef OPENGL_11
      //! Color.
      machete::math::Vec4 color;
#else
      //! Offset (the result will be vert + offset). The offset is applied after the saling/rotation.
      machete::math::Vec2 offset;
      
      //! Pivot. The scaling and rotation will ocour from this pivot point.
      machete::math::Vec2 pivot;
      
      //! Scaling.
      machete::math::Vec2 scale;
      
      //! Rotation in degrees.
      float rotation;
#endif
    };
    
    //! A GLSL program. A combination of a Vertex shader and a Fragment shader.
    class Program {
    public:
      
      //! Creates an empty program.
      Program() { program = 0; projectionSlot = 0; }
      
      //! Destructor.
      ~Program();
      
      //! Prepare the program for use.
      virtual void Use();
      
      //! Unbinds the program.
      virtual void Unuse();
      
      //! Applies a 2D ortho matrix to the current program.
      /*!
       \param width Width of the viewport.
       \param height Height of the viewport.
       */
      virtual void ApplyOrtho(int width, int height);
      
      //! Upload vertexes to the hardware.
      virtual void Upload(Vtx *verts, int vcount, unsigned short* elems, int ecount, const machete::math::Vec4 & color) = 0;
      
      //! Changes the base transformation.
      /*!
       The transformation will be applied after the Projection matrix and before the Model View matrix.
       
       \param base The new base transformation. By default this is an identity matrix.
       */
      virtual void SetBase(const machete::math::Mat4 & base) = 0;
      
      //! Changes the model-view transformation.
      /*!
       The transformation will be applied after the base transformation matrix.
       
       \sa SetBase
       \param modelView The new model-view transformation. By default this is an identity matrix.
       */
      virtual void SetModelView(const machete::math::Mat4 & modelView) = 0;

    protected:
      //! Links a vertex shader and a fragment shader into a program.
      /*!
       \param vtx The vertex shader.
       \param frag The fragment shader.
       \return true if the link was successful. false otherwise.
       */
      bool CreateProgram(Shader *vtx, Shader *frag);
      
      //! The program handle.
      GLuint program;
      
      //! The projection uniform slot. Used to change the 2D ortho matrix.
      GLuint projectionSlot;
      
      //! The vertex shader.
      Shader *vertexShader;
      
      //! The fragment shader.
      Shader *fragmentShader;
    };
    
    //! Shader Manager. Helps in creating singletons of shaders with a Shader pool.
    class ShaderMgr {
    public:
      
      //! Creates a new empty shader manager.
      ShaderMgr();
      
      //! Destructor.
      ~ShaderMgr();
      
      //! Compiles a shader or gets it from the pool.
      /*!
       \param shaderType Shader type.
       \param name Name to give to the shader. It will be saved to the pool with this name.
       \param source Source code of the shader.
       \return A pointer to the new shader. NULL if it could not be compiled.
       */
      Shader *LoadShader(GLenum shaderType, const char *name, const char *source);
      
    protected:
      
      //! Shader Pool.
      machete::data::Hash<machete::data::Str, Shader*> shaders;
    };
    
    //! A texture descriptor.
    struct Tex {
      
      //! Texture identifier.
      unsigned int id;
      
      //! Width of the texture.
      float width;
      
      //! Height of the texture.
      float height;
    };
    
    //! Pool of textures. Simplifies the creation and loading of textures.
    class TextureMgr {
    public:
      
      //! Creates a new Texture Manager.
      TextureMgr() {}
      
      //! Destructor.
      ~TextureMgr();
      
      //! Loads a texture from the resource path.
      /*!
       \param name Name of the image.
       \return A texture. NULL if it could not be loaded.
       */
      struct Tex* LoadTexture(const char *name);
      
      //! Creates a new texture with the specified width and height.
      /*!
       Depending on the platform there is a limit on the width and height of the
       image created.
       
       \param width Width of the texture.
       \param height Height of the texture.
       \return A texture. NULL if it could not be created.
       */
      struct Tex* CreateTexture(int width, int height);
      
    protected:
      
      //! Texture Pool.
      machete::data::Hash<machete::data::Str, struct Tex*> textures;
    };
        
    //! Vertex Renderer. A simple vertex renderer. Draws each Vtx object as requested.
    class VtxRender : public Program {
    public:
      
      //! Creates a new Vertex Renderer.
      VtxRender();
      
      //! Destructor.
      ~VtxRender();
      
      //! Creates a new Vertex Shader.
      Shader *CreateVtxShader();
      
      //! Creates a new Fragment Shader.
      Shader *CreateFrgShader();
      
      //! Start using this program.
      void Use();
      
      //! Stop using this program.
      void Unuse();
      
      //! Upload vertexes to the hardware.
      void Upload(Vtx *verts, int vcount, unsigned short* elems, int ecount, const machete::math::Vec4 & color);
      
      //! Changes the base transformation.
      /*!
       The transformation will be applied after the Projection matrix and before the Model View matrix.
       
       \param base The new base transformation. By default this is an identity matrix.
       */
      void SetBase(const machete::math::Mat4 & base);
      
      //! Changes the model-view transformation.
      /*!
       The transformation will be applied after the base transformation matrix.
       
       \sa SetBase
       \param modelView The new model-view transformation. By default this is an identity matrix.
       */
      void SetModelView(const machete::math::Mat4 & modelView);
      
    protected:
      //! Pivot Slot.
      GLuint pivotSlot;
      
      //! Offset Slot.
      GLuint offsetSlot;
      
      //! Position Slot.
      GLuint positionSlot;
      
      //! Texture Slot.
      GLuint textureSlot;
      
      //! Scale Slot.
      GLuint scaleSlot;
      
      //! Rotation Slot.
      GLuint rotationSlot;
      
      //! Base Slot.
      GLuint baseSlot;
      
      //! ModelView Slot.
      GLuint modelviewSlot;
      
      //! Sampler Slot.
      GLuint samplerSlot;
      
      //! Destination Color Slot.
      GLuint destColorSlot;
      
      //! Base transformation matrix.
      machete::math::Mat4 base;
      
      //! Model-view transformation matrix.
      machete::math::Mat4 modelView;
    };
    
    //! Global Shader Manager.
    extern ShaderMgr* TheShaderMgr;
    
    //! Global Texture Manager.
    extern TextureMgr* TheTextureMgr;
    
    //! Global default vertex shader program.
    extern VtxRender* TheVertexShader;
    
  }
}
