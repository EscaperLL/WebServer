#pragma once
#include"LogStream.h"
#include"MutexLock.h"
#include"NonCopyAble.h"
#include"Thread.h"
#include "Condition.h"

#include <functional>
#include <string>
#include <vector>

class AsyncLogging : public Base::NonCopyAble
{
  public:
     AsyncLogging(const std::string basename, int flushInterval = 2);
  ~AsyncLogging() {
    if (running_) stop();
  }
  void append(const char* logline,int len);
  void start()
  {
      running_ = true;
      thread_.start();
  }
   void stop() {
    running_ = false;
    cond_.notify();
    thread_.join();
  }
  private:
   void threadFunc();
   typedef FixedBuffer<kLargeBuffer> Buffer;
   typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
   typedef std::shared_ptr<Buffer> BufferPtr;
   bool running_;
   std::string basename_;
   Thread thread_;
   MutexLock mutex_;
   Condition cond_;
   BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
int flushInterval_;
};