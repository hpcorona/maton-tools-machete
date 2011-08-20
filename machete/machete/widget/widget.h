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

#include "../data/lists.h"
#include "../draw/meta.h"
#include "../draw/element.h"
#include "../input/touch.h"
#include "../draw/font.h"
#include "../common/log.h"

using namespace machete::draw;

namespace machete {
  namespace widget {
    
#define SCROLL_ELASTICITY 10.0f
    
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
      
      //! Check if it's candidate for only vertical scroll.
      bool IsVertical() { return bottomRight.x == 0; }
      
      //! Check if it's candidate for only horizontal scroll.
      bool IsHorizontal() { return bottomRight.y == 0; }
      
    protected:
      //! Size of the original widget.
      Vec2 size;
      
      //! Draw Size of the current widget.
      Vec2 drawSize;
      
      //! Top left box size.
      Vec2 topLeft;
      
      //! Bottom right box size.
      Vec2 bottomRight;

      //! Vertexes 9 image mode.
      Vtx verts[16];
      
      //! Vertexes 3 image Vertical.
      Vtx vertsV[8];
      
      //! Vertexes 3 image Horizontal.
      Vtx vertsH[8];
      
      //! Elements to create the widget (9-image mode).
      unsigned short elems[54];
      
      //! Elements to create the widget (3-image vertical).
      unsigned short elemsV[18];
      
      //! Elements to create the widget (3-image horizontal).
      unsigned short elemsH[18];
      
      //! Texture.
      unsigned int texture;

    };
    
    class Widget;
    
    //! The widget event adapter to manage events on widgets in a per-application basis.
    class WidgetEventAdapter {
    public:
      
      //! A widget was tapped.
      /*!
       \param widget The widget that was tapped.
       */
      virtual void WidgetTapped(Element *widget) {}
      
      //! A widget is being dragged.
      /*!
       \param widget The widget that is being dragged.
       \param movement The movement offset.
       */
      virtual void WidgetDragged(Element *widget, Vec2 & movement) {}
      
      //! A widget was affected by inertia.
      /*!
       \param widget The widget that is being moved by inertia.
       \param movement The movement offset.
       */
      virtual void WidgetInertia(Element *widget, Vec2 & movement) {}
      
    };
    
    enum WidgetDisplay {
      WidgetNormal,
      WidgetVertical,
      WidgetHorizontal
    };
    
    //! A widget container that can be sized and still looks good using a framed image.
    class Widget : public machete::draw::Container, public machete::input::TouchListener {
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
      
      //! Change the size of the widget. Automatically calls the Invalidate method.
      /*!
       \param width The new width of the widget.
       \param height the new height of the widget.
       */
      void SetSize(float width, float height);
      
      //! Change the size of the widget. Automatically calls the Invalidate method.
      /*!
       \param size The new size of the widget.
       */
      void SetSize(const Vec2 & size);
      
      //! Get the size of the widget.
      /*!
       \return The size of the widget.
       */
      inline Vec2 & GetSize();
      
      //! The user may be making a tap.
      virtual bool TouchTapIntent();
      
      //! The user is definitely not making a tap.
      virtual void TouchTapCancelled();
      
      //! The user has performed a tap.
      virtual void TouchTapPerformed();
      
      //! The widget accepts drag.
      virtual bool TouchAcceptDrag();
      
      //! The user is dragging with his finger around the widget.
      virtual void TouchDrag(Vec2 & move);
      
      //! The user has dragged "violently" and released a finger, causing an inertia.
      virtual void TouchInertia(Vec2 & move);
      
      //! The touch event has ended.
      virtual void TouchEnded();
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      virtual bool TouchEvent(machete::input::Touch *touch);
      
      //! Changes the current event listener.
      /*!
       \param event The new event listener.
       */
      void SetEventListener(WidgetEventAdapter *event);
      
      //! Gets the current event listener.
      /*!
       \return The current event listener.
       */
      WidgetEventAdapter *GetEventListener();
      
      //! Change the widget's display type.
      /*!
       \param type Widget display type.
       */
      void SetDisplay(WidgetDisplay type);
      
    protected:
      
      //! The states supported by this widget.
      Hash<Str, MetaWidget*> states;
      
      //! The current state of the widget.
      MetaWidget* state;
      
      //! The size of the widget.
      Vec2 size;
      
      //! Touch processor.
      machete::input::TouchProcessor touchProc;
      
      //! The event listener for widget events.
      WidgetEventAdapter *event;
      
      //! Widget display type.
      WidgetDisplay display;
      
    };
    
    //! A simple Button Widget, with a Text component inside.
    /*!
     The widget must have a 'normal' and 'pressed' state.
     */
    class Button : public Widget {
    public:

      //! Create a new button.
      Button();
      
      //! Destructor.
      ~Button();
      
      //! Changes the button's label.
      /*!
       \param label The new label.
       */
      void SetLabel(Str & label);
      
      //! Change the button's label.
      /*!
       \param label The new label.
       */
      void SetLabel(const char* label);
      
      //! Change the font of the button.
      /*!
       \param font The new font.
       */
      void SetFont(Font *font);
      
      //! The button was pressed but not released.
      virtual bool TouchTapIntent();
      
      //! The button was not released or was moved (maybe a drag was requested).
      virtual void TouchTapCancelled();
      
      //! The button was pressed.
      virtual void TouchTapPerformed();

      //! The touch event was ended
      virtual void TouchEnded();
      
      //! Invalidates the size.
      void Invalidate();
      
    protected:

      //! Font to be used with this button.
      Font *font;
      
      //! The label for the button.
      Text *label;
      
    };

    //! A simple Scroll Pane Widget.
    /*!
     This widget consist of a frame widget to draw borders, and a vertical and horizontal scroll bar.
     */
    class Scroll : public Widget {
    public:
      
      //! Create a new Scroll pane.
      Scroll(int width, int height);
      
      //! Destructor.
      ~Scroll();
      
      //! Invalidates the size.
      void Invalidate();
      
      //! Draw the texture of the container, then the internal drawing.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void Draw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      
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
      inline int Count() const;
      
      //! Updates this container and it's childs.
      /*!
       \param time The time since the last update, in seconds.
       */
      void Update(float time);
      
      //! Changes the decorators: border, vertical scrollbar, horizontal scrollbar.
      /*!
       \param frame The frame border, drawed after the container.
       \param vScroll The vertical scrollbar.
       \param hScroll The horizontal scrollbar.
       */
      void SetDecorators(Widget *frame, Widget *vScroll, Widget *hScroll);
      
      //! The widget accepts tap intent when alive.
      virtual bool TouchTapIntent();
      
      //! The widget accepts drag.
      virtual bool TouchAcceptDrag();
      
      //! The user is dragging with his finger around the widget.
      void TouchDrag(Vec2 & move);
      
      //! The user has dragged "violently" and released a finger, causing an inertia.
      void TouchInertia(Vec2 & move);
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      virtual bool TouchEvent(machete::input::Touch *touch);
      
      //! Configure the vertical scrollbar.
      /*!
       \param allowVScroll Allow the user to drag the pane vertically.
       \param drawVScroll Draw the vertical scrollbar. If this is false, then the vertical scrollbar will never be drawn.
       \param autoVScroll Auto hide the scrollbar if the user is no longar dragging. If the drawVScroll is false then the scrollbar will never show up.
       */
      void ConfigureVScroll(bool allowVScroll, bool drawVScroll, bool autoVScroll);

      //! Configure the horizontal scrollbar.
      /*!
       \param allowHScroll Allow the user to drag the pane horizontally.
       \param drawHScroll Draw the horizontal scrollbar. If this is false, then the vertical scrollbar will never be drawn.
       \param autoHScroll Auto hide the scrollbar if the user is no longar dragging. If the drawHScroll is false then the scrollbar will never show up.
       */
      void ConfigureHScroll(bool allowHScroll, bool drawHScroll, bool autoHScroll);
      
      //! Configure if the user is able to drag freely or the widget will try to center into a glue point.
      /*!
       \param freeDrag True to allow the user to drag freely.
       */
      void SetFreeDrag(bool freeDrag);
      
      //! Returns if the scroll is in free drag mode.
      /*!
       \return True if the scroll is in free drag mode. False otherwise.
       */
      inline bool IsFreeDrag() const;
      
      //! Add a new glue point.
      /*!
       \param gp The glue point to add.
       */
      void AddGluePoint(const Vec2 & gp);
      
      //! Add a new glue point.
      /*!
       \param x The x position of the glue point.
       \param y The y position of the glue point.
       */
      void AddGluePoint(float x, float y);
      
      //! Center the view at the desired point.
      /*!
       \param cp The center point.
       */
      void CenterView(const Vec2 & cp);
      
      //! Seek gluepoint.
      /*!
       \param dir Seek for a glue point in the desired direction.
       */
      void SeekGluePoint(const Vec2 & dir);
      
      //! Get the current center.
      /*!
       \return The center of the view.
       */
      Vec2 & GetCenter();

    protected:
      
      //! Draw the frame and then the scrollbars.
      /*!
       \param matrix The current transformation matrix. If this will be changed, then you must restore it before returning from this method.
       \param pos Position or Offset to draw elements.
       \param color Tint to apply to all objects.
       \param ctx DrawContext to draw the elements.
       */
      void InternalDraw(const Mat4 & matrix, Vec2 & pos, Vec4 & color, DrawContext *ctx);
      
      //! Calculates the elastic values based on time. (Counter-inertia).
      /*!
       \param time Time elapsed.
       */
      void CalculateElastic(float time);
      
      //! Step closer to the next target glue point.
      /*!
       \param time Time elapsed.
       */
      void StepTarget(float time);
      
      //! The user has dragged "violently" and released a finger, causing an inertia.
      void ElasticMovement(Vec2 & move);
      
      //! Optional frame widget to draw borders.
      Widget *frame;
      
      //! Optional vertical scrollbar.
      Widget *vScroll;
      
      //! Optional horizontal scrollbar.
      Widget *hScroll;

      //! Bottom and Right margins for the Scrollbars.
      Vec2 margin;
      
      //! Elastic offset on the viewport.
      Vec2 elastic;
      
      //! Current center.
      Vec2 center;

      //! Dynamic viewport.
      Dynamic *viewport;
      
      //! Container to clip the real contents.
      Container *container;
      
      //! Drawing vertical scrollbar.
      bool drawVScroll;
      
      //! Drawing horizontal scrollbar.
      bool drawHScroll;
      
      //! Auto hide the vertical scrollbar.
      bool autoVScroll;
      
      //! Auto hide the horizontal scrollbar.
      bool autoHScroll;
      
      //! Allow vertical scroll.
      bool allowVScroll;
      
      //! Allow horizontal scroll.
      bool allowHScroll;

      //! Allow free dragging. If this is false, then it will always auto-center into a glue point.
      bool freeDrag;
      
      //! The widget is centered into a gluepoint.
      bool centered;
      
      //! The widget is currently centering.
      bool centering;
      
      //! Target glue point.
      Vec2 targetGlue;
      
      //! Last inertia movement.
      Vec2 lastInertia;
      
      //! Glue points.
      machete::data::Iterator<Vec2> gluePoints;
      
    };
    
    //! A container with touch support.
    /*!
     Not propertly a widget, but a more lightweight implementation.
     */
    class TouchContainer : public Container, public machete::input::TouchListener {
    public:
      //! Constructor.
      TouchContainer();
      
      //! Destructor.
      ~TouchContainer();
      
      //! The user may be making a tap.
      virtual bool TouchTapIntent();
      
      //! The user is definitely not making a tap.
      virtual void TouchTapCancelled();
      
      //! The user has performed a tap.
      virtual void TouchTapPerformed();
      
      //! See if it accepts drag.
      virtual bool TouchAcceptDrag();
      
      //! The user is dragging with his finger around the widget.
      virtual void TouchDrag(machete::math::Vec2 & move);
      
      //! The user has dragged "violently" and released a finger, causing an inertia.
      virtual void TouchInertia(machete::math::Vec2 & move);
      
      //! The touch has ended.
      virtual void TouchEnded();
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      virtual bool TouchEvent(machete::input::Touch *touch);

      //! Changes the current event listener.
      /*!
       \param event The new event listener.
       */
      void SetEventListener(WidgetEventAdapter *event);
      
      //! Gets the current event listener.
      /*!
       \return The current event listener.
       */
      WidgetEventAdapter *GetEventListener();
      
      //! Update animations.
      virtual void Update(float time);

    protected:
      //! The touch processor.
      machete::input::TouchProcessor touchProc;
      
      //! Event listener.
      WidgetEventAdapter *event;
    };


  }
}
