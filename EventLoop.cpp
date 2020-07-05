#include"EventLoop.h"
#include"Base/Logging.h"

#include <sys/epoll.h>
#include <sys/eventfd.h>
using namespace network;

__thread EventLoop* t_loopInThisThread = 0;

int createEventfd() {
  int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0) {
    LOG << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

    EventLoop::EventLoop(/* args */)
        :looping_(false),
        poller_(new Epoll),
        quit_(false),
        threadId_(CurrentThread::tid()),
        wakeFd_(createEventfd()),
        pwakeupChannel_(new Channel(this,wakeFd_)),
        handPending_(false)
    {
      pwakeupChannel_->setEvents(EPOLLIN|EPOLLET);
      pwakeupChannel_->setReadHander(std::bind(&EventLoop::handleRead,this));
      pwakeupChannel_->setConnHander(std::bind(&EventLoop::handleConn,this));
      poller_->addEvent(pwakeupChannel_,0);
      if (!t_loopInThisThread)
      {
         t_loopInThisThread = this;
      }
      
    }


    void EventLoop::wakeup()
    {
      uint64_t iOne = 1;
      size_t n =sockets::writen(wakeFd_,(char*)&iOne,sizeof(iOne));
      if(n != iOne)
        {
          LOG<<"EventLoop::wakeup() write " << n << " bytes instead of 8";
       }
    }

    void EventLoop::handleRead()
    {
      uint64_t iOne=1;
      size_t n=sockets::readn(wakeFd_,&iOne,sizeof(iOne));
      if(n != iOne)
      {
        LOG<<"EventLoop::handleRead() reads " << n << " bytes instead of 8";
      }
    }

    void EventLoop::handleConn()
    {
       pwakeupChannel_->setEvents(EPOLLIN|EPOLLET);
       updateEvent(pwakeupChannel_);
    }

    EventLoop::~EventLoop()
    {
      //close(pwakeupChannel_->getFd());
      sockets::shutDownWR(pwakeupChannel_->getFd());
      t_loopInThisThread = NULL;
    }

    void EventLoop::loop()
    {
      looping_ = true;
      while (looping_)
      {
        std::vector<Functor> pedingfuns;
        {
           MutexLockGuard lock(mutex_);
          pedingfuns.swap(pedingFunctors_);
          pedingFunctors_.clear();
        }
        std::vector<sp_channel> activeChannels;
        poller_->poll(activeChannels);
        for (auto &iter:activeChannels)
        {
          iter->hanleEvents();
        }
        handPending_ = true;
        for(auto iter:pedingfuns)
        {
          iter();
        }
        handPending_ = false;
        poller_->handleExpired();//chuli chaoshi
      }
      
    }

    
    void EventLoop::quit()
    {
      looping_ = false;
      if(!isInLoopThread())
      {
        wakeup();
      }
    }

    void EventLoop::runInLoop(Functor&& fun)
    {
      if (isInLoopThread())
      {
        fun();
      }
      else
      {
        queInLoop(std::move(fun));
        wakeup();
      }
    }

    void EventLoop::queInLoop(Functor&& fun)
    {
      {
        MutexLockGuard lock(mutex_);
        pedingFunctors_.push_back(fun);
      }
    }