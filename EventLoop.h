#pragma once
#include"Channel.h"
#include"Epoll.h"
#include"Base/Thread.h"
#include"Base/CurrentThread.h"
#include"SocketsOpt.h"
#include<functional>
#include<memory>

#include <iostream>

class EventLoop
{
public:
    typedef std::function<void()> Functor;
    EventLoop(/* args */);
    ~EventLoop();
    void loop();
    void quit();

    bool isInLoopThread()const{
        return threadId_ == CurrentThread::tid();
    }

    void runInLoop(Functor&& fun);
    void queInLoop(Functor&& fun);

    void addEvent(std::shared_ptr<Channel>& channel,int timeOut = 0)
    {
        poller_->addEvent(channel,timeOut);
    }

    void updateEvent(std::shared_ptr<Channel>& channel,int timeout=0)
    {
        poller_->modEvent(channel,timeout);
    }

    void removeEvent(std::shared_ptr<Channel>& channel)
    {
        poller_->remove(channel);
    }

   

private:
    void wakeup();
    void handleRead();
    void handleConn();
private:
    bool looping_;
    std::shared_ptr<network::Epoll> poller_;
    int wakeFd_;
    bool quit_;
    bool handPending_;
    mutable MutexLock mutex_;
    std::vector<Functor> pedingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    std::shared_ptr<Channel> pwakeupChannel_;
    //void doPendingFunctors();
};


