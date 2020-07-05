#pragma once
#include"Base/MutexLock.h"
#include"Base/NonCopyAble.h"
#include<queue>
#include<memory>

class HttpData;

class TimerNode
{
    public:
        TimerNode(std::shared_ptr<HttpData> ptr_requestData,int time_out);
        ~TimerNode();
        TimerNode(TimerNode &t);
        void update(int timeout);
        bool isValid();
        void clearReq();
        void setDeleted() { deleted_ = true; }
        bool isDeleted() const { return deleted_; }
        size_t getExpTime() const { return expiredTime_; }
    private:
        bool deleted_ ;
        size_t expiredTime_;
        std::shared_ptr<HttpData> SPHttpData_;
};

struct TimerCmp{
    bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const
    {
        return a->getExpTime() > b->getExpTime();
    }
};

class  TimerManager
{
public:
    TimerManager(/* args */);
    ~ TimerManager();
    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();
private:
typedef std::shared_ptr<TimerNode> spTimeNode;
std::priority_queue<spTimeNode,std::deque<spTimeNode>,TimerCmp>
    timeNodeQueue_;
};


