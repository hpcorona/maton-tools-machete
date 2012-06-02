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

#include "../../thread/thread.h"
#include "../../platform/platform.h"
#include "../../data/str.h"

#include <tremor/ogg.h>
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>

namespace machete {
  namespace sound {
    namespace backend {
      
      using namespace machete::thread;
      using namespace machete::platform;
      using namespace machete::data;
      
      class SoundBackend {
      public:
        
        virtual bool Initialize() = 0;
        
        virtual void Shutdown() = 0;
        
        virtual void Detach() = 0;
        
        virtual void Attach() = 0;

        virtual unsigned int LoadSound(const char* name) = 0;
        
        virtual unsigned int LoadStream(const char* name) = 0;
        
        virtual bool BufferData(unsigned int buff, int format, char *data, int size, long rate) = 0;
        
        virtual void DeleteBuffer(unsigned int buff) = 0;
        
        virtual void DeleteSource(unsigned int source) = 0;
        
        virtual bool UnqueueBuffer(unsigned int source, unsigned int *buff) = 0;
        
        virtual bool QueueBuffer(unsigned int source, unsigned int buff) = 0;
        
        virtual bool Play(unsigned int source) = 0;
        
        virtual void Stop(unsigned int source) = 0;
        
        virtual void Pause(unsigned int source) = 0;
        
        virtual void Rewind(unsigned int source) = 0;
        
        virtual void SetVolume(unsigned int source, float vol) = 0;
        
        virtual bool IsPlaying(unsigned int source) = 0;
        
        virtual int QueuedBuffers(unsigned int source) = 0;
        
        virtual int ProcessedBuffers(unsigned int source) = 0;
        
        virtual unsigned int CreateSource(float gain) = 0;
        
        virtual unsigned int CreateBuffer() = 0;
        
        virtual void BindBufferToSource(unsigned int buffer, unsigned int source) = 0;
      };
      
      extern SoundBackend *TheSoundBackend;
      
      //! A music buffer.
      struct MusicBuffer {
        
        unsigned int buffer;      //<!  Backend Buffer.
        
        char* data;               //<! Audio data.
        
        bool loaded;              //<! If the buffer is loaded.
        
      };
      
      //! Internal class to manage background music streaming.
      class MusicStreamWorker : public SequenceWorker<MusicBuffer*> {
      public:
        
        //! Creator.
        MusicStreamWorker();
        
        //! Destructor.
        ~MusicStreamWorker();
        
        //! Prepare a music file for playing.
        /*!
         \param name Name of the music file.
         \param preload Preload the music with 3 buffers.
         \return True if everything was ok.
         */
        bool PrepareMusic(const char *name);
        
        void Service();
        
        //! Get the music format;
        int GetFormat() const;
        
      protected:
        
        //! Load a buffer part.
        void LoadPart(MusicBuffer* buff);
        
        //! Load Ogg file.
        void LoadOgg();
        
        //! Close the current ogg.
        void CloseOgg();
        
        //! File handle.
        FILE* handle;
        
        //! Vorbis Stream.
        OggVorbis_File oggStream;
        
        //! Formatting data.
        vorbis_info* vorbisInfo;
        
        //! Audio file name.
        char *name;
        
        //! If the file is loaded.
        bool loaded;
        
        //! Music format.
        int format;
        
      };
      
    }
  }
}
