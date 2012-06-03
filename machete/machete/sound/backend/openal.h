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

#include <tremor/ogg.h>
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>

namespace machete {
  namespace sound {
    namespace backend {
      
      using namespace machete::thread;
      using namespace machete::platform;
      using namespace machete::data;
      
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
        
        //! Clear all pending work.
        void Clear();
        
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
      
      class OpenALSoundBackend : public SoundBackend {
      public:
        
        OpenALSoundBackend();
        
        ~OpenALSoundBackend();
        
        bool Initialize();
        
        void Shutdown();
              
        void Pause();
        
        void Resume();

        unsigned int SoundLoad(const char* name);
        
        unsigned int StreamCreate(const char* name);

        bool BufferData(unsigned int buff, int format, char *data, int size, long rate);
        
        void SourceDelete(unsigned int source);
        
        void BufferDelete(unsigned int buffer);
        
        bool StreamUnqueue(unsigned int source, unsigned int * buffer);
        
        bool StreamQueue(unsigned int source, unsigned int buff);
        
        bool SoundPlay(unsigned int source);
        
        void SoundStop(unsigned int source);
        
        void SoundPause(unsigned int source);
        
        void SoundRewind(unsigned int source);
        
        void SoundSetVolume(unsigned int source, float vol);
        
        bool SoundIsPlaying(unsigned int source);

        bool StreamPlay(unsigned int source);
        
        void StreamStop(unsigned int source);
        
        void StreamPause(unsigned int source);
        
        void StreamRewind(unsigned int source);
        
        void StreamSetVolume(float vol);
        
        bool StreamIsPlaying(unsigned int source);

        int StreamQueuedBuffers(unsigned int source);
        
        int StreamProcessedBuffers(unsigned int source);
        
        unsigned int SoundCreate(unsigned int buffer);
        
        unsigned int BufferCreate();

        void SourceBind(unsigned int source, unsigned int buffer);
        
        void Update(float time);
        
        void StreamUnload();
        
        void StreamPreload();
        
        bool StreamEnsure(unsigned int stream);
        
        void StreamQueueOne(MusicBuffer *buff);
        
        void StreamDelete(unsigned int source);
        
      protected:
        
        //! OpenAL context.
        ALCcontext* context;
        
        //! OpenAL device.
        ALCdevice* device;
        
        //! Music management.
        MusicStreamWorker *worker;
        
        //! Music buffers.
        MusicBuffer buffers[MAX_MUSIC_BUFFERS];
        
        //! Loaded music ID's.
        Hash<unsigned int, Str> streams;
        
        //! Inverted ID's.
        Hash<Str, unsigned int> inverse;
        
        //! Used stream.
        unsigned int usedId;
        
        //! Consecutive number generator.
        unsigned int nextId;
        
        //! Unsigned int music source.
        unsigned int musicSource;
        
        //! The music is paused.
        bool paused;
        
      };
      
    }
  }
}
