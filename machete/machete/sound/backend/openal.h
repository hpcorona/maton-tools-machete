//
//  openal.h
//  machete
//
//  Created by Hilario Perez Corona on 6/1/12.
//  Copyright (c) 2012 Matón Games. All rights reserved.
//

#pragma once

#include "backend.h"

#ifdef TARGET_IOS

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/oalStaticBufferExtension.h>

#endif

#include "../../platform/platform.h"
#include "../../thread/thread.h"
#include "../../data/tree.h"

namespace machete {
  namespace sound {
    namespace backend {
      
      using namespace machete::platform;
      using namespace machete::data;
      
      class OpenALSoundBackend : public SoundBackend {
      public:
        
        OpenALSoundBackend();
        
        ~OpenALSoundBackend();
        
        bool Initialize();
        
        void Shutdown();
              
        unsigned int LoadSound(const char* name);
              
        void Detach();
        
        void Attach();
        
        bool BufferData(unsigned int buff, int format, char *data, int size, long rate);
        
        void DeleteSource(unsigned int source);
        
        void DeleteBuffer(unsigned int buffer);
        
        bool UnqueueBuffer(unsigned int source, unsigned int * buffer);
        
        bool QueueBuffer(unsigned int source, unsigned int buff);
        
        bool Play(unsigned int source);
        
        void Stop(unsigned int source);
        
        void Pause(unsigned int source);
        
        void Rewind(unsigned int source);
        
        void SetVolume(unsigned int source, float vol);
        
        bool IsPlaying(unsigned int source);
        
        int QueuedBuffers(unsigned int source);
        
        int ProcessedBuffers(unsigned int source);
        
        unsigned int CreateSource(float gain);
        
        unsigned int CreateBuffer();

        void BindBufferToSource(unsigned int buffer, unsigned int source);
        
      protected:
        
        //! OpenAL context.
        ALCcontext* context;
        
        //! OpenAL device.
        ALCdevice* device;
        
      };
      
    }
  }
}
