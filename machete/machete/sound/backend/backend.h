//
//  backend.h
//  machete
//
//  Created by Hilario Perez Corona on 6/1/12.
//  Copyright (c) 2012 Matón Games. All rights reserved.
//

#pragma once

#define MAX_SOUNDS  5
#define MAX_MUSIC_BUFFERS   3
#define MUSIC_BUFFER_SIZE   128000

namespace machete {
  namespace sound {
    namespace backend {
      
      class SoundBackend {
      public:
        
        virtual bool Initialize() = 0;
        
        virtual void Shutdown() = 0;
        
        virtual void Pause() = 0;
        
        virtual void Resume() = 0;

        virtual unsigned int SoundLoad(const char* name) = 0;
        
        virtual unsigned int StreamCreate(const char* name) = 0;
        
        virtual bool BufferData(unsigned int buff, int format, char *data, int size, long rate) = 0;
        
        virtual void BufferDelete(unsigned int buff) = 0;
        
        virtual void SourceDelete(unsigned int source) = 0;
        
        virtual bool StreamUnqueue(unsigned int source, unsigned int *buff) = 0;
        
        virtual bool StreamQueue(unsigned int source, unsigned int buff) = 0;
        
        virtual void StreamDelete(unsigned int source) = 0;
        
        virtual bool SoundPlay(unsigned int source) = 0;
        
        virtual void SoundStop(unsigned int source) = 0;
        
        virtual void SoundPause(unsigned int source) = 0;
        
        virtual void SoundRewind(unsigned int source) = 0;
        
        virtual void SoundSetVolume(unsigned int source, float vol) = 0;

        virtual bool StreamPlay(unsigned int source) = 0;
        
        virtual void StreamStop(unsigned int source) = 0;
        
        virtual void StreamPause(unsigned int source) = 0;
        
        virtual void StreamRewind(unsigned int source) = 0;
        
        virtual void StreamSetVolume(float vol) = 0;
        
        virtual bool SoundIsPlaying(unsigned int source) = 0;
        
        virtual bool StreamIsPlaying(unsigned int source) = 0;
        
        virtual int StreamQueuedBuffers(unsigned int source) = 0;
        
        virtual int StreamProcessedBuffers(unsigned int source) = 0;
        
        virtual unsigned int SoundCreate(unsigned int buffer) = 0;
        
        virtual unsigned int BufferCreate() = 0;
        
        virtual void SourceBind(unsigned int source, unsigned int buffer) = 0;
        
        virtual void Update(float time) = 0;
        
      };
      
      extern SoundBackend *TheSoundBackend;
      
    }
  }
}
