#include"ThreadTask.h"

#include<atomic>
#include<condition_variable>
#include<thread>
#include<vector>
#include<queue>
#include<list>
namespace comm_thread
{
    enum Thread_type
    {
        kWorkIndex=0,//工作线程
        kSuplus, //富裕xianc
    };

    struct TaskTag
    {
        Task* ptr_task;
        std::string task_tag;
    };
    
    struct ThreadPoolDelegateModelData
    {
        std::atomic<bool> is_started = false;
        std::vector<std::shared_ptr<ThreadDelegate>> work_thread;
        std::vector<std::shared_ptr<ThreadDelegate>> surplus_thread;

        std::list<Task*> tasks;
        std::list<Task*> delay_task;//第一次失败的队列

        std::condition_variable task_cond;
        std::mutex task_mutex;
        std::list<Task*> incoming_task;//投入的人物列表

        std::mutex active_task_mutex;
        std::list<TaskTag> ative_tasks;//正在执行的任务

        std::thread t_marster;
        int max_task_num = 20000;
        std::atomic<bool> exit=false;
        std::atomic<bool> clear_task = false;
        std::atomic<size_t> task_count = 0;

    };

    class ThreadDelegate
    {
        public:
            ThreadDelegate(std::shared_ptr<ThreadPoolDelegateModel> ptr_pool,Thread_type type)
                :ptr_pool_(ptr_pool),thread_type_(type)
                {
                    t_ = std::thread(&ThreadDelegate::run,this);
                }
            void run();

            void join()
            {
                {
                    std::unique_lock lock(task_mutex_);
                    exit_ = true;
                    cond_.notify_all();
                }
                {
                    if (t_.joinable())
                    {
                        t_.join();
                    }
                    
                }
            }

            bool idle()
            {
                return idle_;
            }

            void start(Task* pTask);
        private:
            std::thread t_;
            std::shared_ptr<ThreadPoolDelegateModel> ptr_pool_;
            std::condition_variable cond_;
            std::mutex task_mutex_;
            std::queue<Task*> tasks_;

            std::atomic<bool> exit_ = false;
            std::atomic<bool> idle_ =true;//空闲
            Thread_type thread_type_ = kWorkIndex;//工作xiancheng
    };
    
namespace
{
    void addActiveTask(const std::unique_ptr<ThreadPoolDelegateModelData>& ptr_data,Task* pTask,const std::string& strTag)
    {
        TaskTag struTaskTag;
        struTaskTag.ptr_task = pTask;
        struTaskTag.task_tag = strTag;
        {
            std::unique_lock<std::mutex> wait(ptr_data->active_task_mutex);
            ptr_data->incoming_task.emplace_back(struTaskTag);
        }
    }
    void removeActiveTask(const std::unique_ptr<ThreadPoolDelegateModelData>& ptr_data,Task* pTask)
    {
        std::unique_lock<std::mutex> wait(ptr_data->active_task_mutex);
        // auto smartPtrCmp=[](ptr_task& pTask1,ptr_task& pTask2)->bool{
        //     std::shared_ptr<Task> sp_task1,sp_task2;
        //     sp_task1 = pTask1.lock();
        //     sp_task2 = pTask2.lock();
        //     if (sp_task1.get() == sp_task2.get())
        //     {
        //        return true;
        //     }
        //     return false;
        // };
        for (auto it =ptr_data->ative_tasks.begin();it != ptr_data->ative_tasks.end();++it)
        {
            if (it->ptr_task == pTask)
            {
                ptr_data->ative_tasks.erase(it);
                break;
            }  
        }
    }
    std::shared_ptr<ThreadDelegate> getThread(std::unique_ptr<ThreadPoolDelegateModelData>& ptr_data,Task*  pTask,int errCode,const std::string task_tag)
    {
        std::list<TaskTag> taskList;
        {
            std::lock_guard<std::mutex> lock(ptr_data->active_task_mutex);
            taskList = ptr_data->ative_tasks;
        }
        bool bInsthread = false;
        for(auto it = taskList.begin(); it != taskList.end();++it)
        {
            if (it->task_tag == task_tag)
            {
                bInsthread = true;
                break;
            }   
        }
        if (!bInsthread)
        {
            for (auto &it :ptr_data->work_thread)
            {
               if (it->idle())
               {
                   return it;
               }
               
            }  
        }
         for (auto &it :ptr_data->surplus_thread)
            {
               if (it->idle())
               {
                   return it;
               }   
            }  
        if (!bInsthread)
        {
            errCode =1;//所有线程都满了
        }
        else
        {
            errCode =2;//已经在线程中
        }
        return nullptr;   
    }
}

void ThreadDelegate::run()
{
    while (!exit_ || !tasks_.empty())
    {
       Task* pTask = nullptr;
       {
           std::unique_lock<std::mutex> lock_wait(task_mutex_);
           if (tasks_.empty())
           {
               if(!idle)
               {
                   idle_ = true;
               }
               if (!exit_)
               {
                   cond_.wait_for(lock_wait,std::chrono::milliseconds(5000));
                   continue;
               }   
           }
           else
           {
               pTask = tasks_.front();
               tasks_.pop();
           }
           
           if (nullptr == pTask)
           {
               continue;
           }
           if(pTask ->time_out_ <= 0)
           {
               pTask->exec();
           }
           else
           {
               auto currunTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
                auto waitTime= currunTime - pTask->task_creatTime;
                if (waitTime.count() > pTask->time_out_)
                {
                    pTask->timeOut();
                }
                else
                {
                    pTask->exec();
                }
                if(thread_type_ == kWorkIndex)
                {
                    removeActiveTask(ptr_pool_->p_,pTask);
                }
                delete pTask;
           }    
       }
    }
    
}
}