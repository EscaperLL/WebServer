#include"Acceptor.h"
#include"Httpdata.h"
#include"SocketsOpt.h"
#include"EventLoop.h"
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>
using namespace sockets;
  Acceptor::Acceptor(EventLoop *loop, int port , bool reuseport)
    :loop_(loop),
    accpetFd_(socket_bind_listen(port)),
    acceptChannel_(new Channel(loop_,accpetFd_)),
    listening_(false)
  {
      if(accpetFd_ < 0)
      {
        LOG<<"socket_bind_listen failed\r\n";
      }
      acceptChannel_->setFd(accpetFd_);
      acceptChannel_->setEvents(EPOLLIN | EPOLLET);
       if (setSocketNonBlocking(accpetFd_) < 0) 
       {
            perror("set socket non block failed");
             abort();
        }     
  }

Acceptor::~Acceptor()
{
    shutDownWR(accpetFd_);
}

void Acceptor::listen()
{
  if(loop_)
  {
    listening_ = true;
    //listing 
    acceptChannel_->setReadHander(std::bind(&Acceptor::handleRead,this));
    loop_->addEvent(acceptChannel_);
  }
}

void Acceptor::handleRead()
{
  struct  sockaddr_in client_addr={0};
  socklen_t client_addr_len = sizeof(client_addr);
  int accept_fd = 0;
  accept_fd = accept(accpetFd_,(struct sockaddr *)&client_addr,
                             &client_addr_len);
   if(sockets::setSocketNonBlocking(accept_fd) <0)
   {
     //close;
   }
   if (newConnectionCb_)
   {
     newConnectionCb_(accept_fd,inet_ntoa(client_addr.sin_addr));
   }

   LOG<<"new connection: "<<inet_ntoa(client_addr.sin_addr);
   
   int DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
   acceptChannel_->setEvents(DEFAULT_EVENT);

   loop_->updateEvent(acceptChannel_);
}