//
//  draw.h
//  machete
//
//  Created by Hilario Perez Corona on 6/15/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file draw.h
//! \brief High Level Drawing Classes

#pragma once

#include "shader.h"

#define MAX_RING 5
#define MAX_SPR 50
#define MAX_VTX (MAX_SPR*4)
#define MAX_IDX (MAX_SPR*6)

namespace machete {
  namespace graphics {
    
    //! Buffer Manager class.
    /*!
     This class manages vertex and index buffers for the entire application.
     
     This helps reduce the load times and maximizes the memory usage of the buffers, by sharing
     all the buffers between all the draw contexts.
     */
    class BufferMgr {
    public:
      
      //! Create a new Buffer Manager.
      /*!
       This will also create all the buffers on OpenGL.
       */
      BufferMgr();
      
      //! Destructor.
      ~BufferMgr();
      
      //! Request the next Buffers.
      void Next();
      
      //! Get the current array vertex buffer.
      /*!
       \return The current array vertex buffer.
       */
      Vtx* GetVtxArrayBuffer();

      //! Get the current array index buffer.
      /*!
       \return The current array index buffer.
       */
      GLushort* GetIdxArrayBuffer();

      //! Get the current vertex buffer.
      /*!
       \return The current vertex buffer.
       */
      GLuint GetVtxBuffer();

      //! Get the current index buffer.
      /*!
       \return The current index buffer.
       */
      GLuint GetIdxBuffer();

      //! Creates a new array vertex buffer.
      /*!
       \return The new array vertex buffer.
       */
      unsigned int CreateVtxBuffer() const;
      
      //! Creates a new array index buffer.
      /*!
       \return The new array index buffer.
       */
      unsigned int CreateIdxBuffer() const;
      
      //! Deletes a buffer.
      /*!
       \param buffer The buffer to delete.
       */
      void DeleteBuffer(unsigned int buffer) const;

    protected:
      //! Creates all the vertex buffers needed.
      void CreateBuffers();
      
      //! Destroys all the vertex buffers that were created.
      void DestroyBuffers();

      //! Array index ring.
      GLushort indicesRing[MAX_RING][MAX_IDX];
      
      //! Current array index.
      GLushort* indices;
      
      //! Array vertex ring.
      Vtx vertexesRing[MAX_RING][MAX_VTX];
      
      //! Current array vertex.
      Vtx* vertexes;
      
      //! Current index buffer.
      GLuint indexBuffer;
      
      //! Index buffer ring.
      GLuint indexBufferRing[MAX_RING];
      
      //! Current vertex buffer.
      GLuint vertexBuffer;
      
      //! Vertex buffer ring.
      GLuint vertexBufferRing[MAX_RING];
      
      //! Current ring index.
      unsigned int ringIdx;
    };
    
    //! Global buffer manager.
    extern BufferMgr *TheBufferMgr;
    
    //! Render Target.
    enum RenderTarget {
      TargetScreen,   //!< Render to Screen.
      TargetTexture   //!< Render to Texture.
    };
    
    //! Draw Context.
    /*!
     This class is responsible for drawing to screen or to texture.
     
     It's only function make things easy to draw.
     
     This class is platform independent.
     
     You should not create instances of this class directly, instead you should use the function CreateDrawContext.
     
     This class is Batch-Based. This means that each draw only accumulates elements into the batch buffer (given by the BufferMgr via TheBufferMgr global variable). And when the buffer is full then all is uploaded to the hardware and a new buffer is used to avoid wait-time.
     
     \sa CreateDrawContext
     */
    class DrawContext {
    public:
      
      //! Creates a new DrawContext class with the specified target.
      /*!
       \param target The target to render.
       */
      DrawContext(RenderTarget target);
      
      //! Initializes the DrawContext.
      /*!
       \param width The width of the render target.
       \param height The height of the render target.
       */
      void Initialize(int width, int height);
      
      //! Destructor.
      ~DrawContext() {}
      
      //! Start using this context. You must call this before using it.
      void Use();
      
      //! Stop using this context. You must call this when you are done using it.
      void Unuse();
      
      //! Clears the target and prepares to draw a new frame.
      void StartFrame();
      
      //! Flushes all pending drawing operations into the hardware to ensure that we have ended the frame.
      /*!
       \sa Draw
       */
      void EndFrame();

      //! Change the model-view transformation for al subsequent calls.
      /*!
       This will flush all previous pending draw calls and start a new batch.
       
       \sa Draw
       \param mv Model-view matrix.
       */
      void ChangeModelView(const machete::math::Mat4 & mv);
      
      //! Send vertexes and elements to the current batch.
      /*!
       \param verts Vertexes to send. This includes pivot, position, color, scale, rotation, etc...
       \param vcount Number of vertexes to send.
       \param elems Elements to send. This includes the relation between the vertexes. We can draw any arbitrary geometry with this, not just quads.
       \param ecount Numer of elements to send.
       \param texId Texture to use.
       */
      void Draw(Vtx *verts, int vcount, unsigned short* elems, int ecount, GLuint texId);
      
      //! Force a flush of the current batch into hardware.
      void Draw();
      
      //! Request the next buffers to TheBufferMgr.
      void NextBuffers();
      
      //! Get the size of the target.
      /*!
       \return The size of the target.
       */
      machete::math::IVec2 GetSize() const;
      
      //! Get the render texture.
      /*!
       \return The render texture, or zero if the render target is the screen.
       */
      unsigned int GetRenderTexture() const { return texture; }
      
    private:
      //! Vertex Renderer Program.
      VtxRender renderer;
      
      //! Render Target.
      RenderTarget target;
      
      //! Frame buffer.
      GLuint framebuffer;
      
      //! Render buffer, only available when rendering to screen.
      GLuint renderbuffer;
      
      //! The render texture, only available when rendering to texture.
      GLuint texture;
      
      //! The current array index buffer.
      GLushort* indices;
      
      //! The current array vertex buffer.
      Vtx* vertexes;
      
      //! The current index buffer.
      GLuint indexBuffer;
      
      //! The current vertex buffer.
      GLuint vertexBuffer;
      
      //! The number of elements accumulated into the batch.
      unsigned int idxCount;
      
      //! The number of vertexes accumulated into the batch.
      unsigned int vtxCount;
      
      //! The size of the target.
      machete::math::IVec2 size;
      
      //! The base matrix to make all look 2D.
      machete::math::Mat4 base;
      
      //! The last texture used.
      GLuint lastTexBind;
    };
    
  }
}

//! Function to create a new Draw Context.
/*!
 There must be one implementation for each platform.
 
 This must be the only way to create new DrawContext instances.
 
 \param target The render target.
 \param width The width of the target.
 \param height The height of the target.
 \return A draw context object.
 */
machete::graphics::DrawContext* CreateDrawContext(machete::graphics::RenderTarget target, int width, int height);
