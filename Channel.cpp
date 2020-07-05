#include"Channel.h"

#include<queue>
#include<cstdlib>
#include<iostream>

Channel::Channel(EventLoop *loop)
    :m_pLoop(loop),m_iFd(0),m_iEvents(0),m_iLastEvents(0)
    {

    }

Channel::Channel(EventLoop *loop,int fd)
    :m_pLoop(loop),m_iFd(fd),m_iEvents(0),m_iLastEvents(0)
    {
        
    }

  Channel::~Channel()
  {

  }

    void Channel::handleRead()
    {
        if(m_funRead)
        {
            m_funRead();
        }
    }

    void Channel::handleWrite()
    {
          if(m_funWrite)
        {
            m_funWrite();
        }
    }
    
    void Channel::handleConn()
    {
          if(m_funConn)
        {
            m_funConn();
        }
    }
