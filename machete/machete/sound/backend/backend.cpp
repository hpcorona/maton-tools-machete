//
//  backend.cpp
//  machete
//
//  Created by Hilario Perez Corona on 6/1/12.
//  Copyright (c) 2012 Matón Games. All rights reserved.
//

#include "backend.h"

namespace machete {
  namespace sound {
    namespace backend {
      
      MusicStreamWorker::MusicStreamWorker() {
        name = new char[80];
        name[0] = 0;
        
        loaded = false;
      }
      
      MusicStreamWorker::~MusicStreamWorker() {
        delete name;
        
        CloseOgg();
      }
      
      void MusicStreamWorker::Service() {
        while (alive) {
          while (!TryLock()) { if (!alive) return; }
          Wait();
          if (!alive) {
            Unlock();
            break;
          }
          
          if (queue->Count() == 0) {
            Unlock();
            continue;
          }
          
          SwapQueues();
          Unlock();
          
          toDo->Reset();
          while (toDo->Next()) {
            MusicBuffer* buff = toDo->GetCurrent()->GetValue();
            LoadPart(buff);
            
            while(!workDone->TryLock()) {}
            done->Append(buff);
            workDone->NotifyAll();
            workDone->Unlock();
          }
          
          toDo->RemoveAll();
        }
        
        toDo->RemoveAll();
        queue->RemoveAll();
        done->RemoveAll();
        
        finished = true;
      }
      
      void MusicStreamWorker::LoadPart(MusicBuffer* buff) {
        int size;
        int section;
        char *data = buff->data;
        
        size = 0;
        while (size < MUSIC_BUFFER_SIZE) {
          int result = ov_read(&oggStream, data + size, MUSIC_BUFFER_SIZE - size, &section);
          
          if (result > 0) {
            size += result;
          } else if (result == 0) {
            break;
          } else {
            LoadPart(buff);
            return;
          }
        }
        
        if (size == 0)  {
          ov_raw_seek(&oggStream, 0);
          
          LoadPart(buff);
          return;
        }
        
        bool err = TheSoundBackend->BufferData(buff->buffer, format, data, size, vorbisInfo->rate);
        if (err == false) {
          std::cout << "THREAD " << err << std::endl;
        }
      }
      
      bool MusicStreamWorker::PrepareMusic(const char *name) {
        while (!TryLock()) {}
        
        CloseOgg();
        
        loaded = false;
        
        Str n = name;
        n.GetChars(this->name, 80);
        
        LoadOgg();
        
        Unlock();
        
        return loaded;
      }
      
      void MusicStreamWorker::CloseOgg() {
        if (!loaded) return;
        
        ov_clear(&oggStream);
        ThePlatform->CloseFile(handle);
        
        loaded = false;
      }
      
      int MusicStreamWorker::GetFormat() const {
        return format;
      }
      
      void MusicStreamWorker::LoadOgg() {
        if (name[0] == 0) return;
        
        loaded = false;
        
        unsigned long size;
        handle = ThePlatform->OpenFile(name, size);
        if (handle <= 0) {
          return;
        }
        
        int result = ov_open(handle, &oggStream, NULL, (long)size);
        if (result < 0) {
          ThePlatform->CloseFile(handle);
          return;
        }
        
        vorbisInfo = ov_info(&oggStream, -1);
        if (vorbisInfo->channels == 1) {
          format = 0x1101; //AL_FORMAT_MONO16;
        } else {
          format = 0x1103; //AL_FORMAT_STEREO16;
        }
        
        loaded = true;
      }
      
    }
  }
}