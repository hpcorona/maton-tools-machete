//
//  null.cpp
//  machete
//
//  Created by Hilario Perez Corona on 21/2/13.
//  Copyright (c) 2013 Matón Games. All rights reserved.
//

#include "null.h"

namespace machete {
	namespace sound {
		namespace backend {
			
			bool NullSoundBackend::Initialize() {
				return true;
			}
			
      void NullSoundBackend::Shutdown() {
				
			}
        
      void NullSoundBackend::Pause() {
      	
      }
        
      void NullSoundBackend::Resume() {
      	
      }

      unsigned int NullSoundBackend::SoundLoad(const char* name) {
      	return 1;
      }
        
      unsigned int NullSoundBackend::StreamCreate(const char* name) {
      	return 1;
      }
        
      bool NullSoundBackend::BufferData(unsigned int buff, int format, char *data, int size, long rate) {
      	return true;
      }
        
      void NullSoundBackend::BufferDelete(unsigned int buff) {
      	
      }
        
      void NullSoundBackend::SourceDelete(unsigned int source) {
      	
      }
        
      bool NullSoundBackend::StreamUnqueue(unsigned int source, unsigned int *buff) {
      	return true;
      }
        
      bool NullSoundBackend::StreamQueue(unsigned int source, unsigned int buff) {
      	return true;
      }
        
      void NullSoundBackend::StreamDelete(unsigned int source) {
      	
      }
        
      bool NullSoundBackend::SoundPlay(unsigned int source) {
      	return true;
      }
        
      void NullSoundBackend::SoundStop(unsigned int source) {
      	
      }
        
      void NullSoundBackend::SoundPause(unsigned int source) {
      	
      }
        
      void NullSoundBackend::SoundRewind(unsigned int source) {
      	
      }
        
      void NullSoundBackend::SoundSetVolume(unsigned int source, float vol) {
      	
      }

      bool NullSoundBackend::StreamPlay(unsigned int source) {
      	return true;
      }
        
      void NullSoundBackend::StreamStop(unsigned int source) {
      	
      }
        
      void NullSoundBackend::StreamPause(unsigned int source) {
      	
      }
        
      void NullSoundBackend::StreamRewind(unsigned int source) {
      	
      }
        
      void NullSoundBackend::StreamSetVolume(float vol) {
      	
      }
        
      bool NullSoundBackend::SoundIsPlaying(unsigned int source) {
      	return false;
      }
        
      bool NullSoundBackend::StreamIsPlaying(unsigned int source) {
      	return false;
      }
        
      int NullSoundBackend::StreamQueuedBuffers(unsigned int source) {
      	return 0;
      }
        
      int NullSoundBackend::StreamProcessedBuffers(unsigned int source) {
      	return 0;
      }
        
      unsigned int NullSoundBackend::SoundCreate(unsigned int buffer) {
      	return 1;
      }
        
      unsigned int NullSoundBackend::BufferCreate() {
      	return 1;
      }
        
      void NullSoundBackend::SourceBind(unsigned int source, unsigned int buffer) {
      	
      }
        
      void NullSoundBackend::Update(float time) {
      	
      }
			
			
		}
	}
}
