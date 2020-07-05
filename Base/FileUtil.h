#pragma once
#include"NonCopyAble.h"
#include<string>
class AppendFile :public Base::NonCopyAble
{
private:
    /* data */
public:
  explicit  AppendFile(std::string strFileName);
    ~AppendFile();
    
    void append(const char* logline,const size_t len);
    void flush();
private:
size_t wirte(const char* logLine,size_t len);
FILE *fp_;
char* buffer_[64*1024];
};
