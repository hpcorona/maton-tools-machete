//
//  sound.h
//  machete
//
//  Created by Hilario Perez Corona on 8/25/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/oalStaticBufferExtension.h>

#include <Tremor/ogg.h>
#include <Tremor/ivorbiscodec.h>
#include <Tremor/ivorbisfile.h>

#include "../data/tree.h"
#include "../data/str.h"
#include "../platform/platform.h"

#ifndef NULL
#define NULL 0
#endif

using namespace machete::data;
using namespace machete::platform;

#define MAX_SOUNDS  5
#define MAX_MUSIC_BUFFERS   3
#define MUSIC_BUFFER_SIZE   48000

namespace machete {
  
  //! Classes related to sound management.
  namespace sound {
    
    //! Represent a buffered ogg sound.
    class Music {
    public:
      //! Creates a new music object.
      /*!
       \param name Name of the music file.
       */
      Music(const char* name);
      
      //! Destructor.
      ~Music();
      
      //! Update the music, loading new packets if needed.
      /*!
       \param time Time increment (not used).
       */
      void Update(float time);
      
      //! Rewind the source.
      void Rewind();
      
      //! Start playing the sound.
      void Play();
      
      //! Pause the sound.
      void Pause();
      
      //! Resume the sound.
      void Resume();
      
      //! Stop the sound.
      void Stop();
      
      //! Check if it's playing.
      /*!
       \return True if it's playing.
       */
      bool IsPlaying();
      
      //! Check if the music was loaded.
      bool IsLoaded() const;
      
    protected:
      
      //! Enqueue next packets.
      /*!
       \param count Number of packets to enqueue.
       \return True if no problem.
       */
      bool Enqueue(int count);
      
      //! Load Ogg file.
      void LoadOgg();
      
      //! File handle.
      FILE* handle;
      
      //! Vorbis Stream.
      OggVorbis_File oggStream;
      
      //! Formatting data.
      vorbis_info* vorbisInfo;
      
      //! Audio file name.
      char *name;
      
      //! The ALuint source.
      unsigned int source;
      
      //! Detects if it's paused.
      bool pause;
      
      //! Music format.
      ALenum format;
      
      //! Sound loaded.
      bool soundLoaded;
      
      //! First run.
      bool firstRun;
      
      //! Music buffers.
      unsigned int buffers[MAX_MUSIC_BUFFERS];
      
      //! Unbufferer.
      unsigned int unbuff[MAX_MUSIC_BUFFERS];
      
      //! Current local music buffer.
      unsigned int currBuffer;
      
      //! Audio data temporal buffer.
      char** audioData;

    };
    
    //! Represents a single sound.
    class Sound {
    public:
      
      //! Creates a new source binded to a buffer.
      /*!
       \param buffer The start buffer.
       \param cat Category flags.
       */
      Sound(ALuint buffer, unsigned int cat);
      
      //! Destructor.
      ~Sound();
      
      //! Rebind a source to a new buffer.
      /*!
       \param buffer The new buffer.
       \param cat The new category flags.
       */
      inline void Rebind(ALuint buffer, unsigned int cat);
      
      //! Rewind the source.
      inline void Rewind();
      
      //! Start playing the sound.
      inline void Play();
      
      //! Pause the sound.
      inline void Pause();
      
      //! Resume the sound.
      inline void Resume();
      
      //! Stop the sound.
      inline void Stop();
      
      //! Check if it's playing.
      /*!
       \return True if it's playing.
       */
      inline bool IsPlaying();
      
      //! Get the current category.
      /*!
       \return The category flags.
       */
      inline unsigned int GetCategory() const;

    private:
      
      //! Category flag.
      unsigned int category;
      
      //! The ALuint source.
      unsigned int source;
      
      //! Detects if it's paused.
      bool pause;
      
    };
    
    //! Global sound manager class definition.
    class SoundManager {
    public:
      
      //! Constructor.
      SoundManager();
      
      //! Destructor.
      ~SoundManager();
      
      //! Initialize the sound manager.
      /*!
      \return True if everything was initialized correctly. False otherwise.
       */
      bool Initialize();
      
      //! Unload sound effects and all devices.
      void Unload();
      
      //! Preload a sound into the buffer.
      /*!
       \param name The sound name.
       \return True if the sound was loaded.
       */
      bool Preload(const char* name);
      
      //! Play a sound in a singleton mode.
      /*!
       \brief If the sound is playing, it will be stoped and restarted.
       \param name Name of the sound to play.
       \param category The category flags.
       \return The source of the sound.
       */
      Sound* PlaySingleton(const char* name, unsigned int category);
      
      //! Play a sound.
      /*!
       \brief If the sound is playing, a new sound will start.
       \param name The name of the sound to play.
       \param category The category flags.
       \return The source of the sound.
       */
      Sound* Play(const char* name, unsigned int category);
      
      //! Load a Music file, more likely a compressed sound mp3.
      /*!
       \param name Name of the music file.
       \return Null if it could not be loaded. The music otherwise.
       */
      Music* LoadMusic(const char* name);
      
    protected:
      
      //! Loads a new sound buffer.
      /*!
       \param name The name of the sound.
       \return The buffer id.
       */
      unsigned int LoadBuffer(const char* name);
      
      //! OpenAL context.
      ALCcontext* context;
      
      //! OpenAL device.
      ALCdevice* device;
      
      //! Loaded buffer sound fx.
      Hash<Str, ALuint> buffers;
      
      //! Sound source singletons.
      Hash<Str, Sound*> singletons;
      
      //! Sounds currently playing. Non-playing sources are moved on to the available list.
      /*!
       \brief Singletons doesn't count here.
       */
      Iterator<Sound*> sounds;
      
      //! Unused sources cache.
      Iterator<Sound*> available;
      
      //! Music cache.
      Hash<Str, Music*> musics;
      
    };
    
    //! Global sound manager.
    extern SoundManager* TheSoundMgr;
    
  }
  
}
