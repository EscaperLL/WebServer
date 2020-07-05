#pragma once
#include"Httpdata.h"
#include"Channel.h"
#include"TimeNode.h"
#include<unordered_map>
#include<sys/epoll.h>
#include<memory>

namespace network
{
    class Epoll
    {
    public:
        Epoll();
        ~Epoll();
        void addEvent(sp_channel request,int timeout);
        void modEvent(sp_channel request,int timeout);
        void remove(sp_channel request);
       void poll( std::vector<sp_channel>  &activeChannels);
        //std::vector<sp_channel> getEventRequest();
        void addTimer(sp_channel request,int timeout);
          void handleExpired();
    private:
        static const int kMaxFds=10000;
        static const int kInitEventListSize = 16;
        int ePollFd_;
         std::vector<epoll_event> events_;
        sp_channel fd2chan_[kMaxFds];
        std::shared_ptr<HttpData> fd2http_[kMaxFds];
        TimerManager timeManger_;

    };
} // namespace network


