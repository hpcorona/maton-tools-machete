//
//  thread.h
//  machete
//
//  Created by Hilario Perez Corona on 12/13/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//
//! \file thread.h
//! \brief Multithreading Management.

#pragma once

#include "../data/lists.h"
#include <pthread.h>

namespace machete {
  
  //! Multithreading classes.
  namespace thread {
    
    //! Multithread work.
    class HeavyWork {
    public:
      
      //! Multithread work.
      virtual void Run() = 0;
      
      //! Runs on the main thread each frame.
      virtual void Update(float time) {};
      
      //! Runs on the main thread when completed.
      virtual void Complete() = 0;
      
    };
    
    //! A thread (internal usage).
    class Thread {
    public:
      
      //! Create a new Thread.
      Thread(HeavyWork *work);
      
      //! Destructor.
      ~Thread();
      
      //! Start the thread.
      bool Start(pthread_attr_t *attr);
      
      //! If the thread has finished.
      bool IsFinished() const;
      
      //! A POSIX thread.
      pthread_t threadId;
      
      //! The actual work to do.
      HeavyWork *work;
      
      //! Detect if a thread has finished running.
      bool finished;
      
    };
    
    //! The actual multithreaded routine.
    void* ThreadRunnerRoutine(void* data);
    
    //! A thread manager for easy multithreading.
    class ThreadManager {
    public:
      
      //! Create a new Thread Manager.
      ThreadManager();
      
      //! Destructor.
      ~ThreadManager();
      
      //! Launch a new Heavy Work (must be run on the main thread).
      bool Launch(HeavyWork *work);
      
      //! Update the threads (must be run on the main thread).
      void Update(float time);
      
    protected:
      
      //! Thread pool.
      machete::data::Iterator<Thread*> pool;
      
      //! Shared thread attributes.
      pthread_attr_t attr;
      
    };
    
    //! The global thread manager.
    extern ThreadManager *TheThreadMgr;
  }
}
