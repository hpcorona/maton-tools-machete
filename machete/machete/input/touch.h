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

namespace machete {
  
  //! Input classes and data structures.
  namespace input {
    
    //! Defines the current state of a Touch event.
    enum TouchPhase {
      TouchStart,     //!< The touch was started.
      TouchMove,      //!< The user is moving the finger.
      TouchStill,     //!< The user is still touching but not moving the finger.
      TouchEnd,       //!< The user is no longer touching the surface. Also this event can enable the tap count.
      TouchCancelled, //!< The touch event was cancelled. Maybe a phone call was received.
      TouchNone       //!< No touch event available.
    };
    
    //! Represents a touch event
    struct Touch {
      //! Creates a new Touch event.
      Touch() : start(0,0), previous(0,0), current(0,0), offset(0,0) {
        reference = 0;
        finger = 0;
        phase = TouchNone;
        tapCount = 0;
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
      
    protected:
      
      //! Touch array.
      Touch **touches;
      
      //! Number of fingers available.
      int touchCount;
      
      //! True if there are no touches to process.
      bool invalid;
    };
    
    //! The global variable to handle touch events.
    extern TouchInput *TheTouchInput;
    
  }
}
