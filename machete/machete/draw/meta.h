//
//  meta.h
//  machete
//
//  Created by Hilario Perez Corona on 6/20/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file meta.h
//! \brief Meta elements, base for other higher level drawing elements.

#pragma once

#include "../graphics/draw.h"
#include "../data/lists.h"
#include "../input/touch.h"

using namespace machete::graphics;
using namespace machete::math;
using namespace machete::data;

//! Non-iOS compiler...
#ifndef TARGET_IOS
namespace machete { namespace widget { class Scroll; } }
#endif

namespace machete {
  
  //! High Level drawing elements, it's purpose is containe easy to use scene elements.
  namespace draw {
    
    //! Abstract class. Used to represent any kind of geometry that can be drawn.
    class MetaElement {
    public:
      
      //! Sends data to the current batch in a DrawContext.
      /*!
       \param ctx The context to wich the data will be sent.
       \param program The program to use. Send NULL to use the default vertex shader.
       \param pivot Pivot to apply rotation and scale.
       \param pos Position offset for the vertexes.
       \param scale Scale.
       \param color Tint color.
       \param rotation Rotation.
       \param flipX If the geometry must be flipped horizontally.
       \param flipY if the geometry must be flipped vertically.
       */
      virtual void Draw(DrawContext *ctx, Program *program, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY) = 0;
      
      //! Get the size of the geometry.
      /*!
       \return The size of the geometry.
       */
      virtual Vec2 & GetSize() = 0;
    };
    
    //! A srite drawing class. It extracts a section of a texture and renders it as two triangles to represent a quad.
    class MetaSprite : public MetaElement {
    public:
      
      //! Create a new sprite.
      /*!
       \param size The size of the sprite in pixels.
       \param uv0 The first coordinate of the texture.
       \param uv1 The second coordinate of the texture.
       \param texture The identifier of the texture.
       */
      MetaSprite(const Vec2 & size, const Vec2 & uv0, const Vec2 & uv1, unsigned int texture);
      
      //! Destructor.
      ~MetaSprite();
      
      //! Sends data to the current batch in a DrawContext.
      /*!
       \param ctx The context to wich the data will be sent.
       \param program The program to use. Send NULL to use the default vertex shader.
       \param pivot Pivot to apply rotation and scale.
       \param pos Position offset for the vertexes.
       \param scale Scale.
       \param color Tint color.
       \param rotation Rotation.
       \param flipX If the geometry must be flipped horizontally.
       \param flipY if the geometry must be flipped vertically.
       */
      void Draw(DrawContext *ctx, Program* program, const Vec2 & pivot, const Vec2 & pos, const Vec2 & scale, const Vec4 & color, float rotation, bool flipX, bool flipY);

      //! Get the size of the geometry.
      /*!
       \return The size of the geometry.
       */
      Vec2 & GetSize() { return size; }
      
      //! Get the starting UV position.
      /*!
       \return The starting UV position.
       */
      Vec2 & GetUV0() { return uv0; }

      //! Get the ending UV position.
      /*!
       \return The ending UV position.
       */
      Vec2 & GetUV1() { return uv1; }
      
      //! Get the texture used for the image.
      /*!
       \return The texture used for the image.
       */
      unsigned int GetTexture() { return texture; }

    protected:
      
      Vtx verts[4],       //!< Vertexes.
          vertsV[4],      //!< Vertexes flipped vertically.
          vertsH[4],      //!< Vertexes flipped horizontally.
          vertsVH[4];     //!< Vertexes flipped in both axis.
      
      //! Elements to create the triangle.
      unsigned short elems[6];
      
      //! Texture.
      unsigned int texture;
      
      //! Size of the sprite.
      Vec2 size;
      
      //! The starting UV position.
      Vec2 uv0;
      
      //! The ending UV position.
      Vec2 uv1;
      
    };
    
    //! Create a new Meta Sprite based on a drawing context.
    /*!
     \param ctx Basic drawing context.
     \return The meta sprite.
     */
    MetaSprite* MetaSpriteFromDrawContext(DrawContext *ctx);
    
    //! Abstract element prividing the basic functionality for scene management.
    class Element : public machete::input::TouchReceiver {
    public:
      
#ifndef TARGET_IOS
      friend class machete::widget::Scroll;
#endif
      
      //! Create a new Element. This is abstract, you need to use other specialiced classes.
      Element();
      
      //! Set the current position of the element.
      /*!
       \param pos The new position.
       */
      void SetPosition(const Vec2 & pos) { position.x = pos.x; position.y = pos.y; }
      
      //! Set the current position of the element.
      /*!
       \param x The x position.
       \param y the y position.
       */
      void SetPosition(float x, float y) { position.x = x; position.y = y; }
      
      //! Set the current pivot of the element.
      /*!
       \param x The x pivot.
       \param y the y pivot.
       */
      void SetPivot(float x, float y) { this->pivot.x = x; this->pivot.y = y; }
      
      //! Set the current pivot of the element.
      /*!
       \param pivot The new pivot.
       */
      void SetPivot(const Vec2 & pivot) { this->pivot.x = pivot.x; this->pivot.y = pivot.y; }
      
      //! Set the rotation of the element. The rotation is made around the pivot.
      /*!
       \param rotation The rotation in degrees.
       */
      void SetRotation(float rotation) { this->rotation = rotation; }
      
      //! Set the scaling of the element. The scale is made around the pivot.
      /*!
       \param x Scale about the x axis.
       \param y Scale about the y axis.
       */
      void SetScale(float x, float y) { scale.x = x; scale.y = y; }
      
      //! Set the scaling of the element. The scale is made around the pivot.
      /*!
       \param scale Scale about x and y axis.
       */
      void SetScale(const Vec2 & scale) { this->scale.x = scale.x; this->scale.y = scale.y; }
      
      //! Set the Tint color.
      /*!
       \param r Normalized Red.
       \param g Normalized Green.
       \param b Normailzed Blue.
       \param a Normalized Alpha.
       */
      void SetColor(float r, float g, float b, float a) { color.x = r; color.y = g; color.z = b; color.w = a; }
      
      //! Set the Tint color.
      /*!
       \param color Normalized color.
       */
      void SetColor(const Vec4 & color) { this->color.x = color.x; this->color.y = color.y; this->color.z = color.z; this->color.w = color.w; }
      
      //! Get the current position.
      /*!
       \return The current position.
       */
      Vec2 & GetPosition() { return position; }

      //! Get the current pivot.
      /*!
       \return The current pivot.
       */
      Vec2 & GetPivot() { return pivot; }
      
      //! Get the current scale.
      /*!
       \return The current scale.
       */
      Vec2 & GetScale() { return scale; }
      
      //! Get the current color.
      /*!
       \return The current color.
       */
      Vec4 & GetColor() { return color; }
      
      //! Get the global bounds. Bubbling up to the root parent.
      /*!
       \return The global bounds.
       */
      Rect2D GetGlobalBounds();
      
      //! Get the local bounds.
      /*!
       \return The local bounds.
       */
      Rect2D & GetBounds() { return bounds; }
      
      //! Changes the behaviour of updating each frame.
      /*!
       \param active If true, then this element will be updated each frame. If false then this will not be updated.
       */
      void SetActive(bool active) { this->active = active; }
      
      //! Check if this element is active.
      /*!
       \return true if the element is active.
       */
      bool IsActive() { return active; }
      
      //! Changes the visibility of this element.
      /*!
       \param visible If false then this element will not be visible.
       */
      void SetVisible(bool visible) { this->visible = visible; }
      
      //! Check if this element is drawn each frame.
      /*!
       This can be true and the element still not visible due to culling or off screen.
       
       \return true if this element is drawn each frame.
       */
      bool IsVisible() { return visible; }
      
      //! Set data associated with this element.
      /*!
       It can be any data. Useful for integrating with other engines (physics maybe?).
       
       \param data Data to associate with this element.
       */
      void SetData(void *data) { this->data = data; }
      
      //! Get the data associated with this element.
      /*!
       \return The data associated.
       */
      void* GetData() { return data; }
      
      //! Removes a child element.
      /*!
       This is only available when the element is a container.
       
       \param child Child to remove.
       */
      void Remove(Element *child) { /* Nada */ }
      
      //! Remove this element from it's parent.
      void RemoveFromParent() { if (parent != NULL) { parent->Remove(this); } parent = NULL; }
      
      //! Sets the current parent for this element.
      /*!
       \param parent The new parent.
       */
      void SetParent(Element *parent) { this->parent = parent; }
      
      //! Get the current parent for this element.
      /*!
       \return The parent of this element.
       */
      Element* GetParent() { return parent; }
      
      //! Deactivates and makes invisible this node.
      /*!
       Equivalent to SetActive(false) and SetVisible(false).
       
       \sa SetActive
       \sa SetVisible
       */
      void Disable() { active = false; visible = false; }
      
      //! Activates and makes visible this node.
      /*!
       Equivalent to SetActive(true) and SetVisible(true).
       
       \sa SetActive
       \sa SetVisible
       */
      void Enable() { active = true; visible = true; }
      
      
      //! Get the user defined type.
      /*!
       \return The user defined type.
       */
      inline unsigned long GetType() { return type; }
      
      //! Get the object id. Assigned automatically each time a new element is created.
      /*!
       \return The object id.
       */
      inline unsigned long GetObjId() { return objId; }
      
      //! Changes the type of object.
      /*!
       \param t New type of object. This can be anything that makes sense for your game.
       */
      void SetType(unsigned long t) { type = t; }
      
      //! Changes the object identifier.
      /*!
       \param o The new id of this object. This can be anything that makes sense for your game.
       */
      void SetObjId(unsigned long o) { objId = o; }
      
      //! Invalidates the object, updating it's bounds.
      /*!
       When you change position of an object, they are not re-calculated. You need to call this function
       to recalculate the bounds.
       
       This is made this way for performance reasons.
       */
      virtual void Invalidate() = 0;
      
      //! Updates this element.
      /*!
       \param time The time since the last update, in seconds.
       */
      virtual void Update(float time) = 0;
      
      //! Draw this element to a drawing context.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      virtual void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx) = 0;
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      virtual bool TouchEvent(machete::input::Touch *touch) { return false; };
      
      //! Change the vertex shader.
      /*!
       \param program The new program to use. Set to NULL to use the default.
       */
      void SetShader(Program *program);
      
    protected:
      
      //! Object type.
      unsigned long type;
      
      //! Object identifier.
      unsigned long objId;
      
      //! If this element is active.
      bool active;
      
      //! If this element is visible.
      bool visible;
      
      //! Current position.
      Vec2 position;
      
      //! The pivot of the element.
      Vec2 pivot;
      
      //! Rotation to apply in degrees.
      float rotation;
      
      //! Scaling factor.
      Vec2 scale;
      
      //! The tint to apply.
      Vec4 color;
      
      //! The parent element.
      Element *parent;
      
      //! The bounds of the element.
      Rect2D bounds;
      
      //! Associated data.
      void *data;
      
      //! The vertex shader to use.
      Program *program;
      
      //! Static zeroe'd vector.
      static Vec2 ZERO2;
      
      //! Static one'd vector.
      static Vec2 ONE2;
      
      //! Current ID to assign to the next object created.
      static unsigned long NEXT_ID;
    };

  }
}
