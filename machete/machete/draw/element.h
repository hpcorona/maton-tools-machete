//
//  element.h
//  machete
//
//  Created by Hilario Perez Corona on 6/17/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file element.h
//! \brief High Level scene elements most commonly used.

#pragma once

#include "../graphics/draw.h"
#include "meta.h"

namespace machete {
  namespace draw {
    
    //! A timed element usually representing a single frame of an animation.
    class TimedElement {
    public:
      
      //! Create a timed element.
      /*!
       \param time Time in seconds.
       \param element Element to be timed.
       */
      TimedElement(float time, Element *element) { this->time = time; this->element = element; }
      
      //! Get the time.
      /*!
       \return The time.
       */
      inline float GetTime() { return time; }
      
      //! Get the timed element.
      /*!
       \return The timed element.
       */
      inline Element* GetElement() { return element; }
      
    protected:
      
      //! Time in seconds.
      float time;
      
      //! Element.
      Element *element;
    };
    
    //! Container for other elements. This helps creating herarchies of elements.
    class Container : public Element {
    public:
      
      //! Create a new empty container.
      Container();
      
      //! Adds an element at the end of the list.
      void Add(Element *child);
      
      //! Removes an element from the list.
      void Remove(Element *child);
      
      //! Switches one element for another.
      /*!
       This is not a swap, to make a swap you would need to do two switches.
       */
      void Switch(Element *prev, Element *elem);
      
      //! Count the childs.
      /*!
       \return The number of immediate childs.
       */
      inline int Count() const { return count; }
      
      //! Invalidates the container, updating it's bounds.
      /*!
       When you change position of an object, or add/remove elements, they are not re-calculated. You need to call this function
       to recalculate the bounds.
       
       This is made this way for performance reasons.
       */
      void Invalidate();
      
      //! Updates this container and it's childs.
      /*!
       \param time The time since the last update, in seconds.
       */
      void Update(float time);
      
      //! Draw this element and it's childs to a drawing context.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      Iterator<Element*> *Iterator();
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      bool TouchEvent(machete::input::Touch *touch);

      
    protected:
      
      //! Number of immediate childs.
      int count;
      
      //! Childs.
      machete::data::Iterator<Element*> childs;
    };
    
    //! The root container.
    /*!
     This can be used as a root container for the screen DrawContext or for a
     texture DrawContext.
     */
    class Root : public Container {
    public:
      
      //! Creates a new Root container that draws to a specified DrawContext.
      /*!
       \param ctx Context to draw.
       */
      Root(DrawContext *ctx);
      
      //! Draw the childs using the context specified in the constructor.
      void Draw();
      
    protected:
      
      //! The target DrawContext.
      DrawContext *context;
      
      //! An identity matrix with no transformation because this is a root node.
      Mat4 matrix;
    };
    
    //! Draws any MetaElement geometry.
    class Drawing : public Element {
    public:
      
      //! Creates a new drawing.
      /*!
       \param e The MetaElement to draw each frame.
       */
      Drawing(MetaElement *e);
      
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
      
      //! Changes the horizontal flip behaviour.
      /*!
       \param flipX true if you want to flip around the x axis.
       */
      void SetFlipX(bool flipX) { this->flipX = flipX; }
      
      //! Changes the vertical flip behaviour.
      /*!
       \param flipY true if you want to flip around the y axis.
       */
      void SetFlipY(bool flipY) { this->flipY = flipY; }
      
      //! Get the current horizontal behaviour.
      /*!
       \return true if the geometry will be flipped on the x axis.
       */
      bool IsFlipX() const { return flipX; }
      
      //! Get the current vertical behaviour.
      /*!
       \return true if the geometry will be flipped on the y axis.
       */
      bool IsFlipY() const { return flipY; }
      
    protected:
      
      //! To flip on x.
      bool flipX;
      
      //! To flip on y.
      bool flipY;
      
      //! The MetaElement to draw each frame.
      MetaElement *element;
    };
    
    //! Draws an animation on screen.
    class Animation : public Element {
    public:
      
      //! Create a new empty animation.
      Animation();
      
      //! Adds a frame to the animation.
      /*!
       \param time Time in seconds.
       \param child Child to display.
       */
      void Add(float time, Element *child);
      
      //! Changes the loop behaviour.
      /*!
       If the animation is not looped, then it will show the last image still when the whole animation is completed.
       
       \param loop Set to true if you want to loop the animation infinitely.
       */
      void SetLoop(bool loop) { this->loop = loop; }
      
      //! Get the loop behaviour.
      /*!
       \return true if the animation is looped.
       */
      inline bool IsLoop() const { return loop; }
      
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
      
      //! Restart the current animation.
      /*!
       If the animation is inactive, then it will show the first frame still.
       */
      void Restart();
      
      //! Check if the animation is finished.
      /*!
       You should check if an animation is finished after updating this node.
       
       If you want to remove an element from scene when the animation is ended, then you must do it before
       it's drawn to screen, because the next call to draw will show the frame 1 agean.
       
       \return true if the animation was ended and restarted. false if it's not finished.
       */
      inline bool IsFinished() const { return finished; }
      
    protected:
      
      //! This animation is looped.
      bool loop;
      
      //! The animation has finished.
      bool finished;
      
      //! Time left on the current frame.
      float time;
      
      //! The current time in seconds.
      float currTime;
      
      //! Total animation time.
      float totalTime;
      
      //! Current frame being rendered.
      LinkedList<TimedElement*> *current;
      
      //! All the frames.
      machete::data::Iterator<TimedElement*> frames;
    };
    
    //! An actor represents a bunch of animations named 'actions'. You can only play one animation or action at a time.
    class Actor : public Element {
    public:
      
      //! Create a new actor.
      Actor();
      
      //! Add a new action.
      /*!
       \param action Action name.
       \param anim Animation linked to the action name.
       */
      void Add(const Str & action, Animation *anim);
      
      //! Get the current fallback animation.
      /*!
       The fallback animation is used to play a "default" animation when another animation is done and not looped.
       
       \return The current fallback animation, or NULL if no fallback animation is present.
       */
      inline Animation *GetFallback() const { return fallback; }
      
      //! Get the current animation being played.
      /*!
       \return The current animation. NULL if no current animation is available.
       */
      inline Animation *GetCurrent() const { return current; }
      
      //! Clears the Fallback animation.
      void ClearFallback() { fallback = NULL; }
      
      //! Check if the current animation is finished.
      /*!
       \sa Animation::IsFinished
       \return true if the animation is finished.
       */
      inline bool IsFinished() const;
      
      //! Check if the current animation is looped.
      /*!
       \sa Animation::IsLoop
       \return true if the animation is looped.
       */
      inline bool IsLoop() const;
      
      //! Change the fallback animation.
      /*!
       \param name The name of the action.
       */
      void SetFallback(const char* name);
      
      //! Changes the loop behaviour for the current animation.
      /*!
       If the animation is not looped, then it will show the last image still when the whole animation is completed.
       
       \sa Animation::SetLoop
       \param loop Set to true if you want to loop the animation infinitely.
       */
      void SetLoop(bool loop);
      
      //! Play an action.
      /*!
       The loop behaviour will be respected on the target animation.
       
       The animation will be restarted before start playing.
       
       \param name Name of the action.
       \return true if the new animation is running. false if it could not be found.
       */
      bool Play(const char* name);
      
      //! Play an action without loop and then play a fallback.
      /*!
       The loop behaviour will be changed to false on the target animation.
       
       The animation will be restarted before start playing.
       
       \param name Name of the action.
       \param fallback Name of the fallback action.
       \return true if the new animation is running. false if it could not be found.
       */
      bool Play(const char* name, const char* fallback);
      
      //! Play an action.
      /*!
       The loop behaviour will be respected on the target animation.
       
       The animation will be restarted before start playing.
       
       \param name Name of the action.
       \return true if the new animation is running. false if it could not be found.
       */
      bool Play(const char* name, bool loop);
      
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
      
      //! Restart the current animation.
      /*!
       \sa Animation::Restart
       */
      void Restart();
      
    protected:
      
      //! Current fallback animation.
      Animation *fallback;
      
      //! Current animation.
      Animation *current;
      
      //! Actions for this actor.
      Hash<Str, Animation*> actions;
    };
    
    //! A dynamic texture generator. Or in other words, an easy method of generating off-screen rendering.
    class Dynamic : public Root {
    public:
      
      //! Create a new dynamic element that render it's childs to a texture.
      /*!
       \param texWidth Texture width.
       \param texHeight Texture height.
       \param size The render size of the final element.
       */
      Dynamic(int texWidth, int texHeight, Vec2 & size);
      
      //! Destructor.
      ~Dynamic();
      
      //! Changes the behaviour of the rendering.
      /*!
       \param use true to render using the dynamic texture, or false to render the child elements directly.
       */
      void SetUsingTexture(bool use) { usingTexture = use; }
      
      //! Check if this element is using the dynamic texture.
      inline bool IsUsingTexture() const { return usingTexture; }
      
      //! Draw the dynamic texture with all it's childs.
      void Draw();
      
      //! Draw this element to a drawing context.
      /*!
       If usingTexture is true, then it will only render the texture. If it's false, then it will render it's childs.
       
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      
      //! Get the texture MetaSprite.
      /*!
       \return The texture MetaSprite.
       */
      MetaSprite *GetMetaSprite() { return texture; };
      
    protected:
      
      //! Use the dynamic texture when rendering.
      bool usingTexture;
      
      //! The dynamic texture as a MetaSprite.
      MetaSprite *texture;
      
    };
    
  }
}
