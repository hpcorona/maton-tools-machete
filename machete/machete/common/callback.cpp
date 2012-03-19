//
//  callback.cpp
//  machete
//
//  Created by Hilario Perez Corona on 3/19/12.
//  Copyright (c) 2012 Matón Supergames. All rights reserved.
//

#include "callback.h"

namespace machete {
  namespace common {
    
    //! Method to invoke the callback.
    void InvokeCallback(struct CallbackData *data) {
      ((data->instance)->*(data->method))(data->data);
    }
    
    void MakeCallback(struct CallbackData &cbData, Callback *instance, CallbackFunc method, void *data) {
      cbData.instance = instance;
      cbData.method = method;
      cbData.data = data;
    }
    
  }
}