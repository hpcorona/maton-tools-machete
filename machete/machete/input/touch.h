//
//  touch.h
//  machete
//
//  Created by Hilario Perez Corona on 6/24/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include "../math/vector.h"

namespace machete {
  namespace input {
    
    /*!
     @enum TouchPhase
     Defines the current state of a Touch event.
     */
    enum TouchPhase {
      
      /*!
       The touch has started.
       */
      TouchStart,
      
      /*!
       The user is moving the finger.
       */
      TouchMove,
      
      /*!
       The user is still touching but not moving the finger.
       */
      TouchStill,
      
      /*!
       The user is no longer touching the surface. Also this event can enable the tap count.
       */
      TouchEnd,
      
      /*!
       The touch event was cancelled. Maybe a phone call was received.
       */
      TouchCancelled,
      
      /*!
       No touch event available.
       */
      TouchNone
    };

    /*!
     @struct Touch
     Represents a Touch event.
     */
    struct Touch {
      /*!
       Constructor.
       */
      Touch() : start(0,0), previous(0,0), current(0,0), offset(0,0) {
        reference = 0;
        finger = 0;
        phase = TouchNone;
        tapCount = 0;
      }
      
      /*!
       @var reference The reference is used to bind a touch event with an object of any kind.
       */
      void* reference;
      
      /*!
       @var finger Useful to number the touch.
       */
      int finger;

      /*!
       @var reference The reference is used to bind a touch event with an object of any kind.
       */
      TouchPhase phase;
      
      /*!
       @var start Coordinate where the TouchStart was created.
       */
      machete::math::Vec2 start;
      
      /*!
       @var previous The previous coordinate.
       */
      machete::math::Vec2 previous;
      
      /*!
       @var current The current coordinate.
       */
      machete::math::Vec2 current;
      
      /*!
       @var offset The change between the previous coordinate and the new one.
       */
      machete::math::Vec2 offset;
      
      /*!
       @var tapCount The number of taps that the user has made. Only available with the TouchEvent phase.
       */
      int tapCount;
    };
    
    /*!
     @class TouchInput
     Manages the Touch events.
     
     Each platform must provide glue code to fill the events declared here.
     */
    class TouchInput {
    public:
      /*!
       @function TouchInput
       Creates a new TouchInput object with 10 touches available.
       */
      TouchInput();
      
      /*!
       @function ~TouchInput
       Destructor.
       */
      ~TouchInput();
      
      /*!
       @function SetTouchCount
       Changes the number of touches currently available.
       
       @param count Number of fingers currently interacting.
       */
      inline void SetTouchCount(int count) { touchCount = count; }
      
      /*!
       @function GetTouchCount
       Get the number of fingers interacting.
       
       @return The number of fingers curently interacting.
       */
      inline int GetTouchCount() const { return touchCount; }
      
      /*!
       @function GetTouch
       Get a Touch from the Touch array.
       
       @param idx The index of the array.
       
       @return The touch at the position of the array.
       */
      inline Touch* GetTouch(int idx) {
        return touches[idx];
      }
      
      /*!
       @function MarkAvailable
       Mark the object as Available.
       
       Use this from the platform specific implementation to tell us that there are updates
       */
      void MarkAvailable() { invalid = false; }
      
      /*!
       @function Invalidate
       Mark all touches as processed.
       */
      void Invalidate() { invalid = true; }
      
      /*!
       @function IsAvailable
       Check if there is information available.
       */
      inline bool IsAvailable() const { return !invalid; }
      
    protected:
      
      /*! @var touches Touch array. */
      Touch **touches;
      
      /*! @var touchCount Number of fingers available */
      int touchCount;
      
      /*! @var invalid True if there are no touches to process. */
      bool invalid;
    };
    
    /*!
     @var TheTouchInput
     The global variable to handle touch events.
     */
    extern TouchInput *TheTouchInput;
    
  }
}
