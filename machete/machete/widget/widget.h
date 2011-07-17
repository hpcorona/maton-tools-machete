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
#include "../draw/element.h"

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
    
    class Widget : public machete::draw::Container {
    public:
      
      Widget();
      Widget(const Vec2 & size);
      
      //! Add a new state.
      /*!
       \param state State name.
       \param meta Meta widget linked to the state name.
       */
      void Add(const Str & state, MetaWidget *meta);
      
      //! Get the current meta widget being drawn.
      /*!
       \return The current meta widget. NULL if no current meta widget is available.
       */
      inline MetaWidget *GetCurrent() const;
      
      //! Clears the current meta widget.
      void Clear();
      
      //! Change the current state.
      /*!
       \param name The name of the state.
       */
      void SetState(const char* name);
      
      //! Invalidates the object, updating it's bounds.
      /*!
       When you change position of an object, they are not re-calculated. You need to call this function
       to recalculate the bounds.
       
       This is made this way for performance reasons.
       */
      void Invalidate();
      
      //! Updates this element.
      /*!
       \param time The time since the last update, in seconds.
       */
      void Update(float time);
      
      //! Draw this element to a drawing context.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      
      //! Change the size of the widget.
      /*!
       \param width The new width of the widget.
       \param height the new height of the widget.
       */
      void SetSize(float width, float height);
      
      //! Change the size of the widget.
      /*!
       \param size The new size of the widget.
       */
      void SetSize(const Vec2 & size);
      
      //! Get the size of the widget.
      /*!
       \return The size of the widget.
       */
      inline Vec2 & GetSize();
      
    private:
      
      //! The states supported by this widget.
      Hash<Str, MetaWidget*> states;
      
      //! The current state of the widget.
      MetaWidget* state;
      
      //! The size of the widget.
      Vec2 size;
      
    };
    
  }
}
