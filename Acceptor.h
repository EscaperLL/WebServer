#pragma once

#include"Channel.h"
#include"Base/NonCopyAble.h"
#include<functional>
#include <netinet/in.h>
#include<string>
class EventLoop;
class sockaddr_in;
class Acceptor : public Base::NonCopyAble
{
    public:
        typedef std::function<void(int ifd,std::string strIp)> NewConnectionCb;
        Acceptor(EventLoop *loop, int port ,bool reuseport);
        ~Acceptor();
        void setNewConnectionCb(const NewConnectionCb& cb){
            newConnectionCb_ = cb;
        }
        bool listenning() const{
            return listening_;
        }
        void listen();
    private:

        void handleRead();
        EventLoop *loop_ ;
        int accpetFd_;
        std::shared_ptr<Channel>  acceptChannel_;
        NewConnectionCb newConnectionCb_;
        bool listening_ ;
};