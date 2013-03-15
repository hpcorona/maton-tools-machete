//
//  null.h
//  machete
//
//  Created by Hilario Perez Corona on 21/2/13.
//  Copyright (c) 2013 Matón Games. All rights reserved.
//

#include "backend.h"

#pragma once

namespace machete {
	namespace sound {
		namespace backend {
			
      class NullSoundBackend : public SoundBackend {
      public:
        
        bool Initialize();
        
        void Shutdown();
        
        void Pause();
        
        void Resume();

        unsigned int SoundLoad(const char* name);
        
        unsigned int StreamCreate(const char* name);
        
        bool BufferData(unsigned int buff, int format, char *data, int size, long rate);
        
        void BufferDelete(unsigned int buff);
        
        void SourceDelete(unsigned int source);
        
        bool StreamUnqueue(unsigned int source, unsigned int *buff);
        
        bool StreamQueue(unsigned int source, unsigned int buff);
        
        void StreamDelete(unsigned int source);
        
        bool SoundPlay(unsigned int source);
        
        void SoundStop(unsigned int source);
        
        void SoundPause(unsigned int source);
        
        void SoundRewind(unsigned int source);
        
        void SoundSetVolume(unsigned int source, float vol);

        bool StreamPlay(unsigned int source);
        
        void StreamStop(unsigned int source);
        
        void StreamPause(unsigned int source);
        
        void StreamRewind(unsigned int source);
        
        void StreamSetVolume(float vol);
        
        bool SoundIsPlaying(unsigned int source);
        
        bool StreamIsPlaying(unsigned int source);
        
        int StreamQueuedBuffers(unsigned int source);
        
        int StreamProcessedBuffers(unsigned int source);
        
        unsigned int SoundCreate(unsigned int buffer);
        
        unsigned int BufferCreate();
        
        void SourceBind(unsigned int source, unsigned int buffer);
        
        void Update(float time);
        
      };
			
		}
	}
}
