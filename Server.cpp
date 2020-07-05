#include"Server.h"
using namespace std;
Server::Server(EventLoop *loop,int port)
    :loop_(loop),
    accept_(new Acceptor(loop_,port,false))
{
    accept_->setNewConnectionCb(std::bind(&Server::newConnection,this,placeholders::_1,placeholders::_2));
}

Server::~Server()
{

}

void Server::newConnection(int fd,std::string strIP)
{
    sockets::setSocketNodelay(fd);
    shared_ptr<HttpData> request_info=make_shared<HttpData>(loop_,fd);
   
    request_info->getChannel()->setHolder(request_info);
    loop_->queInLoop(std::bind(&HttpData::newConnection,request_info));
}

void Server::start()
{
    accept_->listen();
}
