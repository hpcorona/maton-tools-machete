//
//  touch.h
//  machete
//
//  Created by Hilario Perez Corona on 6/24/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file touch.h
//! \brief Touch related input classes and managers.

#pragma once

#include "../math/vector.h"
#include "../anim/func.h"

#ifndef NULL
#define NULL 0
#endif

namespace machete {
  
  //! Input classes and data structures.
  namespace input {
    
    struct Touch;
    
    class TouchReceiver {
    public:
      
      //! Send a touch event to this element.
      /*!
       \param touch The touch event.
       \return True if the event was processed.
       */
      virtual bool TouchEvent(Touch *touch) = 0;
      
    };
    
    //! Defines the current state of a Touch event.
    enum TouchPhase {
      TouchStart = 1,     //!< The touch was started.
      TouchMove = 2,      //!< The user is moving the finger.
      TouchStill = 3,     //!< The user is still touching but not moving the finger.
      TouchEnd = 4,       //!< The user is no longer touching the surface. Also this event can enable the tap count.
      TouchCancelled = 5, //!< The touch event was cancelled. Maybe a phone call was received.
      TouchNone = 0       //!< No touch event available.
    };
    
    //! Represents a touch event
    struct Touch {
      //! Creates a new Touch event.
      Touch() : start(0,0), previous(0,0), current(0,0), offset(0,0) {
        reference = 0;
        finger = 0;
        phase = TouchNone;
        tapCount = 0;
        owner = NULL;
      }
      
      //! The reference is used to bind a touch event with an object of any kind.
      void* reference;
      
      //! Useful to number the touch.
      int finger;
      
      //! The reference is used to bind a touch event with an object of any kind.
      TouchPhase phase;
      
      //! Coordinate where the TouchStart was created.
      machete::math::Vec2 start;
      
      //! The previous coordinate.
      machete::math::Vec2 previous;
      
      //! The current coordinate.
      machete::math::Vec2 current;
      
      //! The change between the previous coordinate and the new one.
      machete::math::Vec2 offset;
      
      //! The current touch processor owner.
      TouchReceiver *owner;
      
      //! Touch next phase (updated if the touch event is not yet processed).
      TouchPhase nextPhase;
      
      //! Accumulated movements (updated if the touch event is not yet processed).
      machete::math::Vec2 movement;
      
      //! Check if it's consumed.
      bool consumed;
      
      //! The number of taps that the user has made. Only available with the TouchEnd phase.
      int tapCount;
    };
 
    //! Manages the Touch events.
    /*!
     Each platform must provide glue code to fill the events declared here.
     */
    class TouchInput {
    public:
      //! Creates a new TouchInput object with 10 touches available.
      TouchInput();
      
      //! Destructor.
      ~TouchInput();
      
      //! Changes the number of touches currently available.
      /*!
       \param count Number of fingers currently interacting.
       */
      inline void SetTouchCount(int count) { touchCount = count; }

      //! Get the number of fingers interacting.
      /*!
       \return The number of fingers curently interacting.
       */
      inline int GetTouchCount() const { return touchCount; }
      
      //! Get a Touch from the Touch array.
      /*!
       \param idx The index of the array.
       \return The touch at the position of the array.
       */
      inline Touch* GetTouch(int idx) {
        return touches[idx];
      }

      //! Mark the object as Available.
      /*!
       Use this from the platform specific implementation to tell us that there are updates
       */
      void MarkAvailable() { invalid = false; }
      
      //! Mark all touches as processed.
      void Invalidate() { invalid = true; }
      
      //! Check if there is information available.
      inline bool IsAvailable() const { return !invalid; }
      
      //! Cancel all touch events.
      void CancelTouchEvents();
      
      //! Set the tap tolerance for movement.
      /*!
       How much can i move my finger and still consider my gesture as a tap?
       \param tolerance Toleranse, measured in pixels.
       */
      void SetTapTolerance(float tolerance);
      
      //! Get the current tap tolerance.
      float GetTapTolerance() const;
      
      //! Get the squared tap tolerance.
      float GetTapToleranceSquared() const;
      
    protected:
      
      //! Touch array.
      Touch **touches;
      
      //! Number of fingers available.
      int touchCount;
      
      //! True if there are no touches to process.
      bool invalid;
      
      //! Tap tolerance.
      float tolerance;
    };
    
    //! An event listener. Receives updates about how the user is touching the device on a Widget.
    class TouchListener {
    public:
      
      //! The touch was cancelled.
      virtual void TouchCancelled() = 0;
      
      //! See if it accepts taps.
      virtual bool TouchAcceptTap() = 0;
      
      //! The user may be making a tap.
      virtual bool TouchTapIntent() = 0;
      
      //! The user is definitely not making a tap.
      virtual void TouchTapCancelled() = 0;
      
      //! The user has performed a tap.
      virtual void TouchTapPerformed() = 0;
      
      //! See if it accepts drag.
      virtual bool TouchAcceptDragX() = 0;
      
      //! See if it accepts drag.
      virtual bool TouchAcceptDragY() = 0;
      
      //! The user has started dragging the widget from the specified position.
      virtual void TouchStartDrag(machete::math::Vec2 & position) = 0;
      
      //! The user is dragging with his finger around the widget.
      virtual void TouchDrag(machete::math::Vec2 & move) = 0;
      
      //! The user has dragged "violently" and released a finger, causing an inertia.
      virtual void TouchInertia(machete::math::Vec2 & move) = 0;
      
      //! The touch has ended.
      virtual void TouchEnded() = 0;
      
    };
    
    //! Touch processor detector. Will try to detect simple touch gestures.
    class TouchProcessor {
    public:
      
      //! Creates a new touch processor.
      /*!
       \param listener The event listener.
       */
      TouchProcessor(TouchListener *listener);
      
      //! Send a touch event to the processor.
      /*!
       \param touch The touch event.
       \param bounds The bounds against wich the processor will check gestures.
       \return True if the event was processed.
       */
      bool Gather(Touch *touch, const machete::math::Rect2D & bounds);
      
      //! Acquiere a touch event.
      /*!
       \param owner The touch receiver.
       */
      void Acquiere(TouchReceiver *owner);
      
      //! Release a touch event from an owner, making it ownerless.
      void Release();
      
      //! Update the inertia, if any applies.
      /*!
       \param time The time in seconds.
       */
      void Update(float time);
      
      //! Detects if it's still running inertia.
      /*!
       \return True if it's still moving.
       */
      bool IsAlive() const;
      
      //! Detect if it's tracking touch.
      bool IsTracking() const;
      
      //! Stops the inertia.
      void Stop();
      
    protected:
      
      //! The touch event listener.
      TouchListener *listener;
      
      //! Flag to know if there is a tap event.
      bool withTap;
      
      //! Flag to know if we are tracking a touch.
      bool tracking;
      
      //! Current touch event.
      Touch* touch;
      
      //! Inertia.
      machete::math::Vec2 inertia;
      
      //! Inertia time.
      float time;
      
      //! Is still moving by inertia.
      bool alive;
      
    };
    
    //! The global variable to handle touch events.
    extern TouchInput *TheTouchInput;
    
  }
}
