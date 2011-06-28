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
     \brief Defines the current state of a Touch event.
     */
    enum TouchPhase {
      TouchStart,   //!< The touch was started.
      TouchMove,    //!< The user is moving the finger.
      TouchStill,   //!< The user is still touching but not moving the finger.
      TouchEnd,     //!< The user is no longer touching the surface. Also this event can enable the tap count.
      TouchCancelled, //!< The touch event was cancelled. Maybe a phone call was received.
      TouchNone     //!< No touch event available.
    };

    /*!
     \brief Represents a Touch event.
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
       \brief The reference is used to bind a touch event with an object of any kind.
       */
      void* reference;
      
      /*!
       brief Useful to number the touch.
       */
      int finger;

      /*!
       \brief The reference is used to bind a touch event with an object of any kind.
       */
      TouchPhase phase;
      
      /*!
       \brief Coordinate where the TouchStart was created.
       */
      machete::math::Vec2 start;
      
      /*!
       \brief The previous coordinate.
       */
      machete::math::Vec2 previous;
      
      /*!
       \brief The current coordinate.
       */
      machete::math::Vec2 current;
      
      /*!
       \brief The change between the previous coordinate and the new one.
       */
      machete::math::Vec2 offset;
      
      /*!
       \brief The number of taps that the user has made. Only available with the TouchEnd phase.
       */
      int tapCount;
    };
    
    /*!
     \brief Manages the Touch events.
     
     Each platform must provide glue code to fill the events declared here.
     */
    class TouchInput {
    public:
      /*!
       \brief Creates a new TouchInput object with 10 touches available.
       */
      TouchInput();
      
      /*!
       \brief Destructor.
       */
      ~TouchInput();
      
      /*!
       \brief Changes the number of touches currently available.
       
       \param count Number of fingers currently interacting.
       */
      inline void SetTouchCount(int count) { touchCount = count; }
      
      /*!
       \brief Get the number of fingers interacting.
       
       \return The number of fingers curently interacting.
       */
      inline int GetTouchCount() const { return touchCount; }
      
      /*!
       \brief Get a Touch from the Touch array.
       
       \param idx The index of the array.
       
       \return The touch at the position of the array.
       */
      inline Touch* GetTouch(int idx) {
        return touches[idx];
      }
      
      /*!
       \brief Mark the object as Available.
       
       Use this from the platform specific implementation to tell us that there are updates
       */
      void MarkAvailable() { invalid = false; }
      
      /*!
       \brief Mark all touches as processed.
       */
      void Invalidate() { invalid = true; }
      
      /*!
       \brief Check if there is information available.
       */
      inline bool IsAvailable() const { return !invalid; }
      
    protected:
      
      /*! \brief touches Touch array. */
      Touch **touches;
      
      /*! \brief touchCount Number of fingers available */
      int touchCount;
      
      /*! \brief invalid True if there are no touches to process. */
      bool invalid;
    };
    
    /*!
     \brief The global variable to handle touch events.
     */
    extern TouchInput *TheTouchInput;
    
  }
}
