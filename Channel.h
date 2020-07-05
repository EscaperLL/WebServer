#pragma once

#include"../myWeb/Base/NonCopyAble.h"
#include <sys/epoll.h>
#include<functional>
#include <memory>
#include <string>
#include<unordered_map>

class EventLoop;
class HttpData;

class Channel:public std::enable_shared_from_this<Channel>
{
private:
    typedef std::function<void()> Callback;
    EventLoop* m_pLoop;
     int m_iFd;
  __uint32_t m_iEvents;
  __uint32_t m_iRevents;
  __uint32_t m_iLastEvents;

  std::weak_ptr<HttpData> m_pHolder;

  private:
  Callback m_funRead;
  Callback m_funWrite;
  Callback m_funErr;
  Callback m_funConn;
public:
    Channel(EventLoop *loop);
    Channel(EventLoop *loop,int fd);
    ~Channel();

    int getFd(){return m_iFd;}
    void setFd(int fd){m_iFd = fd;}
    void setHolder(std::shared_ptr<HttpData> &holer){m_pHolder = holer;}
    std::shared_ptr<HttpData> getHolder()
    {
        std::shared_ptr<HttpData> ptr(m_pHolder.lock());
        return ptr;
    }

    void setReadHander(Callback&& read ){m_funRead = read;}
    void setWriteHander(Callback && wirte){m_funWrite =wirte;}

    void setConnHander(Callback&& conn){m_funConn =conn;}

   // void setErrHander(Callback&& err){m_funErr =err;}

    void hanleEvents(){
        m_iEvents = 0;
        if((m_iRevents & EPOLLHUP) && !(m_iRevents & EPOLLIN))
        {
            //bei gua duan !ke du
            m_iEvents = 0;
            return;
        }
        if (m_iRevents & EPOLLERR)
        {
            if (m_funErr)
            {
                m_funErr();
            }
             m_iEvents = 0;
            return;
        }
        if(m_iRevents &( EPOLLIN | EPOLLPRI | EPOLLHUP))
        {
            //kedu youjinjishuju bei gua duan
            handleRead();
        }
         if (m_iRevents & EPOLLOUT) {
             //kexie
            handleWrite();
    }
        handleConn();
    }
    void handleRead();
    void handleWrite();
    
    void handleConn();
    void handleError(int fd, int err_num, std::string short_msg);

     void setRevents(__uint32_t ev) { m_iRevents = ev; }

  void setEvents(__uint32_t ev) { m_iEvents = ev; }
  __uint32_t &getEvents() { return m_iEvents; }

  bool EqualAndUpdateLastEvents() {
    bool ret = (m_iLastEvents == m_iEvents);
    m_iLastEvents = m_iEvents;
    return ret;
  }

  __uint32_t getLastEvents() { return m_iLastEvents; }
};

typedef std::shared_ptr<Channel> sp_channel;

