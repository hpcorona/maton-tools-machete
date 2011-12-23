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
    /*!
     A single task performed once.
     */
    class HeavyWork {
    public:
      
      //! Multithread work.
      virtual void Run() = 0;
      
      //! Runs on the main thread each frame.
      virtual void Update(float time) {};
      
      //! Runs on the main thread when completed.
      virtual void Complete() = 0;
      
    };
    
    //! A resource for synchronization.
    class Resource {
    public:
      
      //! Create a new resource.
      Resource();
      
      //! Destructor.
      ~Resource();
      
      //! Try to lock a resource.
      /*!
       \return True if it was locked, false if the resource is buisy.
       */
      virtual bool TryLock();
      
      //! Unlock a resource.
      virtual void Unlock();
      
      //! Notify just one waiting object.
      virtual void Notify();
      
      //! Notify all waiting objects.
      virtual void NotifyAll();
      
      //! Wait for a signal.
      virtual void Wait();
      
    protected:
      
      //! Condition signal.
      pthread_cond_t condition;
      
      //! Mutex.
      pthread_mutex_t mutex;
      
      //! Time to wait.
      struct timespec _time_to_wait;
      
      //! Nanoseconds to wait.
      unsigned long towait;
    };
    
    //! Worker.
    /*!
     A constant worker will live for a long period of time.
     
     It's like a service or daemon. Maybe for sound buffering, GPU tasks, etc...
     
     It may sinchronize using mutexes and notifiers.
     */
    class BackgroundWorker : public Resource {
    public:
      
      //! Run the service in a loop.
      virtual void Service() = 0;
      
      //! If the thread has finished.
      bool IsFinished() const;
      
      //! If the thread is alive.
      bool IsAlive() const;
      
      //! Start the thread.
      bool Start(pthread_attr_t *attr);

      //! Turn off the alive flag and sends a signal.
      void Shutdown();
      
    protected:
      
      //! If the service is finished.
      bool finished;
      
      //! Control if the worker should be still alive.
      bool alive;
      
      //! The POSIX thread.
      pthread_t threadId;
      
    };
    
    template<class T>
    class SequenceWorker : public BackgroundWorker {
    public:
      
      //! Create a new Sequence Worker.
      SequenceWorker() {
        queue = new machete::data::Iterator<T>();
        toDo = new machete::data::Iterator<T>();
        done = new machete::data::Iterator<T>();
        
        workDone = new Resource();
      }
      
      //! Put a task on the pile.
      void Push(T & task) {
        while (!TryLock()) {}
        
        queue->Append(task);
        
        Unlock();
      }
      
      //! Put several tasks on the pile.
      void Push(machete::data::Iterator<T> * tasks) {
        while (!TryLock()) {}
        
        tasks->Reset();
        
        while (tasks->Next()) {
          queue->Append(tasks->GetCurrent()->GetValue());
        }
        
        Unlock();
      }
      
      //! Send a signal to work.
      void Work() {
        NotifyAll();
      }
      
      //! Get the done resource.
      Resource* GetDoneResource() {
        return workDone;
      }
      
      //! Get the done tasks iterator. Please lock the done resource first.
      machete::data::Iterator<T> * GetDoneTasks() {
        return done;
      }
      
    protected:
      
      //! Swap the queues to reduce mutex lock times.
      void SwapQueues() {
        machete::data::Iterator<T> *mid = queue;
        
        queue = toDo;
        
        toDo = mid;
      }
      
      //! Current queue.
      machete::data::Iterator<T> *queue;
      
      //! Background queue. Will be swapped when working.
      machete::data::Iterator<T> *toDo;
      
      //! Queue for the work done.
      machete::data::Iterator<T> *done;
      
      //! Work done resource.
      Resource *workDone;
      
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
    
    //! The background worker routine.
    void* WorkerRunnerRoutine(void* data);
    
    //! A thread manager for easy multithreading.
    class ThreadManager {
    public:
      
      //! Create a new Thread Manager.
      ThreadManager();
      
      //! Destructor.
      ~ThreadManager();
      
      //! Launch a new Heavy Work (must be run on the main thread).
      bool Launch(HeavyWork *work);
      
      //! Starts a backgrund worker.
      bool Start(BackgroundWorker *worker);
      
      //! Update the threads (must be run on the main thread).
      void Update(float time);
      
    protected:
      
      //! Thread pool.
      machete::data::Iterator<Thread*> pool;
      
      //! Shared thread attributes.
      pthread_attr_t attr;
      
      //! Shared condition attributes.
      pthread_condattr_t attrc;
      
      //! Shared mutex attributes.
      pthread_mutexattr_t attrm;
      
    };
    
    //! The global thread manager.
    extern ThreadManager *TheThreadMgr;
  }
}
