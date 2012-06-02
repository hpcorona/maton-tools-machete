//
//  openal.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/1/12.
//  Copyright (c) 2012 Matón Games. All rights reserved.
//

#include "openal.h"

namespace machete {
  namespace sound {
    namespace backend {
      
#ifdef TARGET_IOS
      ALvoid  alBufferDataStatic(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq) {
        static	alBufferDataStaticProcPtr	proc = NULL;
        
        if (proc == NULL) {
          proc = (alBufferDataStaticProcPtr) alGetProcAddress((const ALCchar*) "alBufferDataStatic");
        }
        
        if (proc)
          proc(bid, format, data, size, freq);
        
        return;
      }
#endif
      
      OpenALSoundBackend::OpenALSoundBackend() {
        device = NULL;
        context = NULL;
      }
      
      OpenALSoundBackend::~OpenALSoundBackend() {
        Shutdown();
      }
      
      bool OpenALSoundBackend::Initialize() {
        if (device != NULL || context != NULL) {
          return true;
        }
        
        device = alcOpenDevice(NULL);
        
        if (device == NULL) {
          return false;
        }
        
        context = alcCreateContext(device, NULL);
        
        alcMakeContextCurrent(context);
        
        ALfloat listPos[] = { 0, 0, 0 };
        ALfloat listVel[] = { 0, 0, 0 };
        ALfloat listOri[] = { 0, 0, -1, 0, 1, 0 };
        
        alListenerfv(AL_POSITION, listPos);
        alListenerfv(AL_VELOCITY, listVel);
        alListenerfv(AL_ORIENTATION, listOri);
        
        return true;
      }
      
      void OpenALSoundBackend::Shutdown() {
        if (context != NULL) {
          alcMakeContextCurrent(NULL);
          
          alcDestroyContext(context);
          
          context = NULL;
        }
        
        if (device != NULL) {
          alcCloseDevice(device);
          
          device = NULL;
        }
      }
      
      void OpenALSoundBackend::Detach() {
        alcSuspendContext(context);
        alcMakeContextCurrent(NULL);
      }
      
      void OpenALSoundBackend::Attach() {
        alcMakeContextCurrent(context);
        alcProcessContext(context);
      }
      
      unsigned int OpenALSoundBackend::LoadSound(const char *name) {
        return ThePlatform->LoadAudio(name);
      }
      
      bool OpenALSoundBackend::BufferData(unsigned int buff, int format, char *data, int size, long rate) {
        alBufferDataStatic(buff, format, data, size, rate);
        int err = alGetError();
        return err == AL_NO_ERROR;
      }
      
      bool OpenALSoundBackend::QueueBuffer(unsigned int source, unsigned int buff) {
        alSourceQueueBuffers(source, 1, &buff);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      bool OpenALSoundBackend::UnqueueBuffer(unsigned int source, unsigned int * buffer) {
        alSourceUnqueueBuffers(source, 1, buffer);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      void OpenALSoundBackend::DeleteSource(unsigned int source) {
        alDeleteSources(1, &source);
      }
      
      void OpenALSoundBackend::DeleteBuffer(unsigned int buffer) {
        alDeleteBuffers(1, &buffer);
      }
      
      bool OpenALSoundBackend::Play(unsigned int source) {
        alSourcePlay(source);
        
        return alGetError() == AL_NO_ERROR;
      }
      
      void OpenALSoundBackend::Stop(unsigned int source) {
        alSourceStop(source);
      }
      
      void OpenALSoundBackend::Pause(unsigned int source) {
        alSourcePause(source);
      }
      
      void OpenALSoundBackend::Rewind(unsigned int source) {
        alSourceRewind(source);
      }
      
      void OpenALSoundBackend::SetVolume(unsigned int source, float vol) {
        alSourcef(source, AL_GAIN, vol);
      }
      
      bool OpenALSoundBackend::IsPlaying(unsigned int source) {
        ALint val;
        
        alGetSourcei(source, AL_SOURCE_STATE, &val);
        return val == AL_PLAYING;
      }
      
      int OpenALSoundBackend::QueuedBuffers(unsigned int source) {
        int queued;
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        return queued;
      }
      
      int OpenALSoundBackend::ProcessedBuffers(unsigned int source) {
        int processed;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
        return processed;
      }
      
      unsigned int OpenALSoundBackend::CreateSource(float gain) {
        unsigned int source;
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, 1.0f);
        alSourcef(source, AL_GAIN, gain);
        alSourcei(source, AL_LOOPING, 0);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        
        return source;
      }
      
      unsigned int OpenALSoundBackend::CreateBuffer() {
        unsigned int buffer;
        alGenBuffers(1, &buffer);
        
        return buffer;
      }
      
      void OpenALSoundBackend::BindBufferToSource(unsigned int buffer, unsigned int source) {
        alSourcei(source, AL_BUFFER, buffer);
      }
      
      SoundBackend* TheSoundBackend = NULL;

    }
  }
}
