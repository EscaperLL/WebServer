#pragma once
#include"FileUtil.h"
#include"MutexLock.h"
#include"NonCopyAble.h"

#include <memory>
#include <string>

class LogFile:Base::NonCopyAble
{
private:
    /* data */
public:
    LogFile(const std::string &baseName,int );
    ~LogFile();

    void append(const char* logline,int len);
    void flush();
    bool rollFile();
    
    private:
  void append_unlocked(const char* logline, int len);

  const std::string basename_;
  const int flushEveryN_;

  int count_;
  std::unique_ptr<MutexLock> mutex_;
  std::unique_ptr<AppendFile> file_;
};
