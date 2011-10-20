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
#include "../data/tree.h"
#include "../data/str.h"
#include "../platform/platform.h"

#ifndef NULL
#define NULL 0
#endif

using namespace machete::data;
using namespace machete::platform;

#define MAX_SOUNDS  5

namespace machete {
  
  //! Classes related to sound management.
  namespace sound {
    
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
      
    };
    
    //! Global sound manager.
    extern SoundManager* TheSoundMgr;
    
  }
  
}
