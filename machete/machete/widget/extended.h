//
//  extended.h
//  machete
//
//  Created by Hilario Perez Corona on 12/6/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//
//! \file extended.h
//! \brief Extended widgets.

#pragma once

#include "widget.h"

namespace machete {
  namespace widget {
    
    //! A widget change adapter to listen for widget changes.
    class WidgetChangedAdapter {
    public:
      
      //! The value of the widget was changed.
      /*!
       \param widget The widget.
       */
      virtual void WidgetValueChanged(Element *widget) {};
      
      //! The widget has lost focus.
      /*!
       \param widget The widget.
       */
      virtual void WidgetFocusLost(Element *widget) {};
      
    };
    
    //! A simple class to display configurable Progress bars.
    class Progressbar : public Container {
    public:
      
      //! Create a new progressbar.
      /*!
       \param outer Outer frame widget.
       \param inner Inner frame widget.
       */
      Progressbar(Widget *outer, Widget *inner);
      
      //! Destructor.
      ~Progressbar();
      
      //! Change the borders.
      /*!
       \param vSep Vertical separators (left & right).
       \param hSep Horizontal separators (up & down).
       */
      void SetBorders(Vec2 & vSep, Vec2 & hSep);
      
      //! Set the minimum and maximum values.
      /*!
       \param min The minimum value.
       \param max The maximum value.
       */
      void SetRange(float min, float max);
      
      //! Get the minimum value.
      /*!
       \return The minimum value.
       */
      float GetMin() const;
      
      //! Get the maximum value.
      /*!
       \return The maximum value.
       */
      float GetMax() const;
      
      //! Set the value.
      /*!
       \param val The value.
       */
      void SetValue(float val);
      
      //! Get the value.
      /*!
       \return The current value.
       */
      float GetValue() const;
      
      //! Get the current porcentile.
      /*!
       \return The current porcentile.
       */
      float GetPorcentile() const;
      
      //! Set the current porcentile.
      /*!
       \param porc The porcentile.
       */
      void SetPorcentile(float porc);
      
      //! Increment the current value.
      /*!
       \param diff The increment value.
       */
      void Increment(float diff);
      
      //! Invalidate.
      void Invalidate();
      
      //! Set the widget size.
      /*!
       \param size The new widget size.
       */
      void SetSize(Vec2 & size);
      
      //! Set the widget size.
      /*!
       \param w The new width.
       \param h The new height.
       */
      void SetSize(float w, float h);
      
    protected:
      
      //! Inner widget (the actual widget that changes size).
      Widget *inner;
      
      //! Outer widget (the widget that doesn't changes size).
      Widget *outer;
      
      //! Current value of the progressbar. Will be trimmed between min and max.
      float value;
      
      //! Porcentile of the progressbar.
      float porcentile;
      
      //! The minimum value of the progressbar.
      float min;
      
      //! The maximum value of the progressbar.
      float max;
      
      //! The horizontal border (left and right).
      Vec2 hBorder;
      
      //! The vertical border (up and down).
      Vec2 vBorder;
      
      //! Widget size.
      Vec2 size;
      
      //! Range.
      Vec2 range;
      
    };
    
    //! Interactive progressbar with a know that the user can move.
    /*!
     \brief The knob widget must have the following states: 'normal', 'off', 'on', 'drag'.
     */
    class Trackbar : public Progressbar, WidgetEventAdapter {
    public:
      
      //! Create a new trackbar.
      /*!
       \param outer Outer frame widget.
       \param inner Inner frame widget.
       \param offset Knob position offset.
       */
      Trackbar(Widget *outer, Widget *inner, Actor *knob);
      
      //! Destructor.
      ~Trackbar();
      
      //! Invalidate the data.
      void Invalidate();
      
      //! Start of a drag event.
      /*!
       \param widget The widget that is beign dragged.
       \param position The initial position.
       */
      void WidgetStartDrag(Element *widget, Vec2 & position);
      
      //! A widget is being dragged.
      /*!
       \param widget The widget that is being dragged.
       \param movement The movement offset.
       */
      void WidgetDragged(Element *widget, Vec2 & movement);
      
      //! The end of a touch event.
      /*!
       \param widget The widget that has ended a touch event on.
       */
      void WidgetEndTouch(Element *widget);
      
      //! Change the value listener.
      /*!
       \param listener Widget listener.
       */
      void SetChangeListener(WidgetChangedAdapter *listener);

    protected:
      
      //! The knob.
      Actor *knob;
      
      //! Touch container.
      Widget *knobCont;
      
      //! Knob offset inside the knob container.
      Vec2 offset;
      
      //! Value per pixel.
      float vpp;
      
      //! Widget change listener.
      WidgetChangedAdapter *changeListener;
      
    };
    
    //! An button based on an animation.
    class ActorButton : public TouchContainer {
    public:
      
      //! Create a new actor button.
      /*!
       \param actor The actor.
       */
      ActorButton(Actor *actor);
      
      //! Destructor.
      ~ActorButton();
      
      //! The touch was cancelled.
      virtual void TouchCancelled();
      
      //! The user may be making a tap.
      virtual bool TouchTapIntent();
      
      //! The user is definitely not making a tap.
      virtual void TouchTapCancelled();
      
      //! The user has performed a tap.
      virtual void TouchTapPerformed();
      
      //! The touch has ended.
      virtual void TouchEnded();
      
    protected:
      
      //! The actor.
      Actor *actor;
      
    };
    
  }
}
