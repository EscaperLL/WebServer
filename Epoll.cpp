#include"Epoll.h"
#include<memory>
#include <assert.h>
using namespace std;


typedef shared_ptr<Channel> SP_Channel;

namespace network
{
const int kEventNum = 4096;
const int kEpollTime = 1000;

    Epoll::Epoll() : ePollFd_(epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize)
    {
        assert(ePollFd_>0);
    }

    Epoll::~Epoll()
    {}

    void Epoll::addEvent(sp_channel request,int timeout)
    {
        int fd= request->getFd();
        if (timeout > 0)
        {
            addTimer(request,timeout);
            fd2http_[fd] = request->getHolder();
        }
        epoll_event struEvent;
        struEvent.data.fd = fd;
        struEvent.events = request->getEvents();

        request->EqualAndUpdateLastEvents();
        fd2chan_[fd] = request;
        if (epoll_ctl(ePollFd_, EPOLL_CTL_ADD, fd, &struEvent) < 0) {
            perror("epoll_add error");
            fd2chan_[fd].reset();
        }
    }
        void Epoll::modEvent(sp_channel request,int timeout)
        {
            if (timeout > 0)
            {
                addTimer(request,timeout);
            }
            int fd= request->getFd();
            if (!request ->EqualAndUpdateLastEvents())
            {
               epoll_event struEvent;
               struEvent.data.fd = fd;
               struEvent.events = request->getEvents();
               if (epoll_ctl(ePollFd_, EPOLL_CTL_MOD, fd, &struEvent) < 0) {
                    perror("epoll_Mod error");
                    fd2chan_[fd].reset();
                }
            }
        }
        void Epoll::remove(sp_channel request)
        {
            int fd= request->getFd();
            epoll_event struEvent;
            struEvent.data.fd = fd;
            struEvent.events = request->getLastEvents();
            if (epoll_ctl(ePollFd_, EPOLL_CTL_DEL, fd, &struEvent) < 0) {
                    perror("epoll_Mod error");
                    fd2chan_[fd].reset();
                }
            fd2chan_[fd].reset();
            fd2http_[fd].reset();
        }
       void Epoll::poll(std::vector<sp_channel>  &activeChannels)
        {
            int event_num = epoll_wait(ePollFd_,&*events_.begin(),events_.size(),kEpollTime);//wait event 
            if (events_.size() == event_num)
            {
                events_.resize(event_num*2);
            }
            
            for(int i =0; i< event_num; ++i)
            {
                int fd = events_[i].data.fd;
                sp_channel cur_channel = fd2chan_[fd];
                if(cur_channel)
                {
                    cur_channel->setRevents(events_[i].events);
                    cur_channel->setEvents(0);
                    activeChannels.emplace_back(cur_channel);
                }
                else
                {
                    // log
                }
            }   
        }
        //std::vector<sp_channel> getEventRequest();
        void Epoll::addTimer(sp_channel request,int timeout)
        {
            std::shared_ptr<HttpData> pHttp= request->getHolder();
            if(pHttp)
                timeManger_.addTimer(pHttp,timeout);
            else
            {
                //http shixiao
            }
            
        }
        void Epoll::handleExpired()
        {
            timeManger_.handleExpiredEvent();
        }


}