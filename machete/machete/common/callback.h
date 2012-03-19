//
//  callback.h
//  machete
//
//  Created by Hilario Perez Corona on 3/19/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#pragma once

namespace machete {
  namespace common {

    //! Just a callback class marker.
    class Callback {
    public:
      virtual void DoCallback(void *) = 0;
    };
    
    //! The Callback function prototype.
    typedef void (Callback:: *CallbackFunc)(void *);

    //! Callback data to store.
    struct CallbackData {
      Callback *instance;
      CallbackFunc method;
      void *data;
    };
        
    //! Method to invoke the callback.
    void InvokeCallback(struct CallbackData *data);
    
    void MakeCallback(struct CallbackData &cbData, Callback *instance, CallbackFunc method, void *data);
    
  }
}