#include"Httpdata.h"
#include"EventLoop.h"
const __uint32_t DEFAULT_EVENT = EPOLLIN | EPOLLET | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;              // ms

HttpData::HttpData(EventLoop* loop,int fd)
    :loop_(loop),
    channel_(new Channel(loop,fd))
    {
        channel_->setReadHander(std::bind(&HttpData::handleRead,this));
        channel_->setWriteHander(std::bind(&HttpData::handleWrite,this));
        channel_->setConnHander(std::bind(&HttpData::handleConn,this));
    }

HttpData::~HttpData()
{

}

void HttpData::newConnection()
{
    channel_->setEvents(DEFAULT_EVENT);
    loop_->addEvent(channel_,DEFAULT_EXPIRED_TIME);
}

void HttpData::handleClose()
{
    sockets::shutDownWR(channel_->getFd());
}

void HttpData::handleRead()
{

}

void HttpData::handleWrite()
{

}

void HttpData::handleConn()
{

}

void HttpData::handleErr(int fd,int err_num,std::string short_msg)
{
    
}