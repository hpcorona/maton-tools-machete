//
//  analytics.h
//  machete
//
//  Created by Hilario Perez Corona on 3/29/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#pragma once

#include "../data/str.h"

namespace machete {
  namespace analytics {
    
    using namespace machete::data;
    
    //! Google analytics tracker (GANTracker).
    class GoogleTracker {
    public:
      
      //! Create a new tracker.
      GoogleTracker();
      
      //! Destructor.
      ~GoogleTracker();
      
      //! Start the Tracker.
      void Start(Str accountId, int dispatchPeriod);
      
      //! Sets the referrer.
      void SetReferrer(Str campaign, Str source, Str medium);
      
      //! Set a custom variable.
      void SetCustomVariable(int idx, Str name, Str value);
      
      //! Track an event.
      void TrackEvent(Str category, Str action, Str label, int value);
      
      //! Track a pageview.
      void TrackPageview(Str page);
      
      //! Stop the tracker.
      void Stop();
      
      //! Dispatch async.
      void Dispatch();
      
    protected:
      
      //! Easily check if the tracker is started.
      bool started;
      
    };
    
  }
}

