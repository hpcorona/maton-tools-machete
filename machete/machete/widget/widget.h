//
//  widget.h
//  machete
//
//  Created by Hilario Perez Corona on 7/3/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file widget.h
//! \brief Simple widgets.

#pragma once

#include "../draw/meta.h"

namespace machete {
  namespace widget {
    
    class MetaWidget : machete::draw::MetaElement {
    public:
      
      //! Create a new widget.
      /*!
       \param size The size of the widget in pixels.
       \param uv0 The first coordinate of the texture.
       \param uv1 The second coordinate of the texture.
       \param topLeft Top left corner size in pixels.
       \param bottomRight Bottom right corner size in pixels.
       \param texture The identifier of the texture.
       */
      MetaWidget(const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, const Vec2 & topLeft, const Vec2 & bottomRight, unsigned int texture);
      
      //! Sends data to the current batch in a DrawContext.
      /*!
       \param ctx The context to wich the data will be sent.
       \param pivot Pivot to apply rotation and scale.
       \param pos Position offset for the vertexes.
       \param scale Scale.
       \param color Tint color.
       \param rotation Rotation.
       \param flipX If the geometry must be flipped horizontally.
       \param flipY if the geometry must be flipped vertically.
       */
      void Draw(DrawContext *ctx, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY);
      
      //! Get the base size of the geometry.
      /*!
       \return The base size of the geometry.
       */
      Vec2 & GetSize();
      
      //! Get the size of the current widget.
      /*!
       \return The size of the current widget.
       */
      Vec2 & GetDrawSize();
      
      //! Set the draw size of the subsequents draw calls.
      /*!
       \param drawSize The new draw size.
       */
      void SetDrawSize(const Vec2 & drawSize);
      
      //! Reset the Draw Size to the original widget size.
      void Reset();
      
    protected:
      //! Size of the original widget.
      Vec2 size;
      
      //! Draw Size of the current widget.
      Vec2 drawSize;
      
      //! Top left box size.
      Vec2 topLeft;
      
      //! Bottom right box size.
      Vec2 bottomRight;

      //! Vertexes.
      Vtx verts[16];
      
      //! Elements to create the widget (9-image mode).
      unsigned short elems[54];
      
      //! Texture.
      unsigned int texture;

    };
    
  }
}
