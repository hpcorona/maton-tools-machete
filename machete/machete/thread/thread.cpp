//
//  thread.cpp
//  machete
//
//  Created by Hilario Perez Corona on 12/13/11.
//  Copyright 2011 Matón Supergames. All rights reserved.
//

#include "thread.h"
#include <time.h>
#include <iostream>
#include "../platform/platform.h"

namespace machete {
  namespace thread {
    
    HeavyWork::~HeavyWork() {}
    
    Resource::Resource() {
      pthread_cond_init(&condition, NULL);
      pthread_mutex_init(&mutex, NULL);
    }
    
    Resource::~Resource() {
      pthread_cond_destroy(&condition);
      pthread_mutex_destroy(&mutex);
    }
    
    bool Resource::TryLock() {
      return pthread_mutex_trylock(&mutex) == 0;
    }
    
    void Resource::Unlock() {
      pthread_mutex_unlock(&mutex);
    }
    
    void Resource::Notify() {
      pthread_cond_signal(&condition);
    }
    
    void Resource::NotifyAll() {
      pthread_cond_broadcast(&condition);
    }
    
    void Resource::Wait() {
      pthread_cond_wait(&condition, &mutex);
    }
    
    bool BackgroundWorker::IsFinished() const {
      return finished;
    }
    
    bool BackgroundWorker::IsAlive() const {
      return alive;
    }
    
    bool BackgroundWorker::Start(pthread_attr_t *attr) {
      finished = false;
      alive = true;
      
      int v = pthread_create(&threadId, attr, &WorkerRunnerRoutine, this);
      
      if (v != 0) {
        finished = true;
        alive = false;
      }
      
      return v == 0;
    }
    
    void BackgroundWorker::Shutdown() {
      while (!TryLock()) {}
      
      alive = false;
      
      NotifyAll();
      
      Unlock();
    }
    
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
      
      pthread_exit(NULL);
      
      return NULL;
    }
    
    void* WorkerRunnerRoutine(void* data) {
      BackgroundWorker* worker = static_cast<BackgroundWorker*>(data);
      
      worker->Service();
      
      pthread_exit(NULL);
      
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
      
      pthread_exit(NULL);
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
    
    bool ThreadManager::Start(BackgroundWorker *work) {
      return work->Start(&attr);
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
