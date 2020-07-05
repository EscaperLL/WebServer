#include"FileUtil.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

AppendFile::AppendFile(string fileName)
:fp_(fopen(fileName.c_str(),"ae"))
{
    //setbuffer(fp_,buffer_,sizeof(buffer_));
}

AppendFile::~AppendFile()
{
  fclose(fp_);
}

void AppendFile::append(const char* logline,const size_t len)
{
    size_t n =wirte(logline,len);
    size_t remainLen = len - n;
    while (remainLen > 0)
    {
        /* code */
        size_t x = wirte(logline+n,remainLen);
        if (0 == x)
        {
           int err = ferror(fp_);
           if (err)
           {
                fprintf(stderr, "AppendFile::append() failed !\n");
           }
           break;
        }
        n +=x;
        remainLen =len - n;
    }
    
}

void AppendFile::flush()
{
  fflush(fp_);
}

size_t AppendFile::wirte(const char* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite_unlocked(logline, 1, len, fp_);
}
