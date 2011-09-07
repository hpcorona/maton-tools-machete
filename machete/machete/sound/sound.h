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

namespace machete {
  
  //! Classes related to sound management.
  namespace sound {
    
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
       */
      void PlaySingleton(const char* name);
      
      //! Play a sound.
      /*!
       \brief If the sound is playing, a new sound will start.
       */
      void Play(const char* name);
      
    protected:
      
      //! Creates a new source.
      /*!
       \param name The buffer name.
       \return The source.
       */
      ALuint NewSource(const char* name);
      
      //! OpenAL context.
      ALCcontext* context;
      
      //! OpenAL device.
      ALCdevice* device;
      
      //! Loaded buffer sound fx.
      Hash<Str, ALuint> buffers;
      
      //! Sound source singletons.
      Hash<Str, ALuint> singletons;
    };
    
    //! Global sound manager.
    extern SoundManager* TheSoundMgr;
    
  }
  
}
