#pragma once

#include"Channel.h"
#include"EventLoop.h"
#include"Acceptor.h"
#include<memory>

class Server
{
private:
    /* data */
public:
    Server(EventLoop *loop,int port);
    ~Server();
    void newConnection(int fd,std::string strIP);
    void start();
private:
    EventLoop* loop_;
    std::unique_ptr<Acceptor> accept_;
    bool start_;
};


