#pragma once
#include"MutexLock.h"
#include"NonCopyAble.h"

#include<pthread.h>
#include<time.h>
#include <cstdint>
#include<error.h>
class Condition : public Base::NonCopyAble  
{
private:
    /* data */
public:
   explicit  Condition(MutexLock &mutex)
   :mutex_(mutex)
   {
       pthread_cond_init(&cond, NULL);
   }
    ~Condition ()
    {
        pthread_cond_destroy(&cond);
    }
    void wait(){ pthread_cond_wait(&cond,mutex_.get());}
    void notify(){pthread_cond_signal(&cond);}
     void notifyAll() { pthread_cond_broadcast(&cond); }
  bool waitForSeconds(int seconds) {
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += static_cast<time_t>(seconds);
    return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex_.get(), &abstime);
  }
    private:
     MutexLock &mutex_;
    pthread_cond_t cond;
};


