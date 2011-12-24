//
//  sound.h
//  machete
//
//  Created by Hilario Perez Corona on 8/25/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

//! \file sound.h
//! \brief Raw sound management, and OGG file loading/buffering and playing.

#pragma once

#ifdef TARGET_IOS

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <OpenAL/oalStaticBufferExtension.h>

#elif TARGET_ANDROID

#include <AL/al.h>
#include <AL/alc.h>
#define alBufferDataStatic alBufferData

#endif

#include <tremor/ogg.h>
#include <tremor/ivorbiscodec.h>
#include <tremor/ivorbisfile.h>

#include "../data/tree.h"
#include "../data/str.h"
#include "../platform/platform.h"
#include "../thread/thread.h"

#ifndef NULL
#define NULL 0
#endif

using namespace machete::data;
using namespace machete::platform;

#define MAX_SOUNDS  5
#define MAX_MUSIC_BUFFERS   3
#define MUSIC_BUFFER_SIZE   128000

namespace machete {
  
  //! Classes related to sound management.
  namespace sound {
    
    //! A music buffer.
    struct MusicBuffer {
      
      unsigned int buffer;      //! OpenAL buffer.
      
      char* data;               //! Audio data.
      
      bool loaded;              //! If the buffer is loaded.
      
    };
    
#ifdef TARGET_ANDROID
    size_t android_fread(void *ptr, size_t size, size_t nmemb, void *datasource);
    int android_fseek(void *datasource, ogg_int64_t offset, int whence);
    int android_fclose(void *datasource);
    long android_ftell(void *datasource);
#endif
    
    //! Internal class to manage background music streaming.
    class MusicStreamWorker : public machete::thread::SequenceWorker<MusicBuffer*> {
    public:
      
#ifdef TARGET_ANDROID
      
      friend size_t android_fread(void *ptr, size_t size, size_t nmemb, void *datasource);
      friend int android_fseek(void *datasource, ogg_int64_t offset, int whence);
      friend int android_fclose(void *datasource);
      friend long android_ftell(void *datasource);
      
#endif
      
      MusicStreamWorker();
      
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
      ALenum GetFormat() const;
      
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
      ALenum format;
      
      //! Maximum position within the file.
      long maxPos;

    };
    
#ifdef TARGET_ANDROID
    
    const ov_callbacks android_ogg_callbacks = {
      android_fread, android_fseek, android_fclose, android_ftell
    };
    
#endif
    
    //! Represent a buffered ogg sound.
    class Music {
    public:
      //! Creates a new music object.
      Music();
      
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
      
      //! Change the volume.
      void SetVolume(float volume);
      
      //! Check if it's playing.
      /*!
       \return True if it's playing.
       */
      bool IsPlaying();
      
      //! Check if the music was loaded.
      bool IsLoaded() const;
      
      //! Prepare a music file for playing.
      /*!
       \param name Name of the music file.
       \return True if everything was ok.
       */
      bool PrepareMusic(const char *name);
      
      //! Unload buffers.
      void Unload();
      
      //! Preload all buffers.
      void Preload();
      
    protected:
      
      //! Create the sound buffers.
      void CreateBuffers();
      
      //! Enqueue next packets.
      /*!
       \param buff The buffer to enqueue.
       */
      void Enqueue(MusicBuffer* buff);
      
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
      MusicBuffer buffers[MAX_MUSIC_BUFFERS];
      
      //! Music stream worker.
      MusicStreamWorker *worker;
      
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

      //! Change the volume.
      void SetVolume(float volume);

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
      
      //! Change the global music volume.
      /*!
       \param volume The new music volume.
       \param category Category of the volume.
       */
      void SetVolume(float volume, unsigned int category);
      
      //! Get the current volume.
      /*!
       \param category Category of the volume.
       \return The current volume.
       */
      float GetVolume(unsigned int category);

      
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
      
      //! Categories volumes.
      Hash<unsigned int, float> volumes;
      
    };
    
    enum MusicFlag {
      PlayInstant = 0,
      PlayFade = 1
    };
    
    //! A music manager.
    class MusicManager {
    public:
      
      //! Create a new Music Manager.
      MusicManager();
      
      //! Destructor.
      ~MusicManager();
      
      //! Play a music file on stream mode.
      /*!
       \param file File name to play.
       \param flags Music flags.
       \param fade Fade time.
       */
      void Play(const char *file, unsigned int flags, float fade);
      
      //! Change the global music volume.
      /*!
       \param volume The new music volume.
       */
      void SetVolume(float volume);
      
      //! Get the current volume.
      /*!
       \return The current volume.
       */
      float GetVolume();
      
      //! Update the music.
      /*!
       \param time The time differential.
       */
      void Update(float time);
      
    protected:
      
      //! The current volume.
      float volume;
      
      //! Music Buffer 1.
      Music* buff1;
      
      //! Music Buffer 2.
      Music* buff2;
      
      //! Current playing Music.
      Music* current;
      
      //! Fading music.
      Music* fading;
      
      //! Current elapsed time used for fading.
      float time;
      
      //! Maximum fade time.
      float maxTime;
      
      //! Current music name.
      Str currentMusicName;
      
    };
    
    //! Global sound manager.
    extern SoundManager* TheSoundMgr;
    
    //! Global music manager.
    extern MusicManager* TheMusicMgr;
    
  }
  
}
