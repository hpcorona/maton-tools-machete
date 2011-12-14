//
//  thread.cpp
//  machete
//
//  Created by Hilario Perez Corona on 12/13/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "thread.h"

namespace machete {
  namespace thread {
    
    Thread::Thread(HeavyWork *work) {
      this->work = work;
      finished = false;
    }
    
    Thread::~Thread() {
      pthread_detach(threadId);
      delete work;
    }
    
    bool Thread::Start(pthread_attr_t *attr) {
      return pthread_create(&threadId, attr, &ThreadRunnerRoutine, this) == 0;
    }
    
    bool Thread::IsFinished() const {
      return finished;
    }
    
    void* ThreadRunnerRoutine(void* data) {
      Thread* thread = static_cast<Thread*>(data);
      
      thread->work->Run();
      
      thread->finished = true;
      
      return NULL;
    }
    
    ThreadManager::ThreadManager() {
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
    
    ThreadManager::~ThreadManager() {
      pthread_attr_destroy(&attr);
      
      pool.Reset();
      pool.Next();
      int total = pool.Count();
      for (int i = 0; i < total; i++) {
        Thread *t = pool.GetCurrent()->GetValue();
        
        delete t;
        
        pool.RemoveCurrent(true);
      }
    }
    
    bool ThreadManager::Launch(HeavyWork *work) {
      Thread *thread = new Thread(work);
      
      if (!thread->Start(&attr)) {
        delete thread;
        
        return false;
      }
      
      pool.Append(thread);
      
      return true;
    }
    
    void ThreadManager::Update(float time) {
      if (pool.Count() == 0) return;
      
      pool.Reset();
      bool hasMore = pool.Next();
      
      while (hasMore) {
        Thread *thread = pool.GetCurrent()->GetValue();
        
        if (thread->IsFinished()) {
          thread->work->Complete();
          
          pool.RemoveCurrent(true);
          
          hasMore = pool.GetCurrent() != NULL;
        } else {
          thread->work->Update(time);
          
          hasMore = pool.Next();
        }
      }
      
    }
    
    ThreadManager *TheThreadMgr = NULL;
    
  }
}
