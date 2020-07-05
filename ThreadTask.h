#include<chrono>
#include<memory>
#include<string>
namespace comm_thread
{
    class Task
    {
        friend class ThreadPoolDelegateModel;
        friend class ThreadDelegate;
    public:
        Task(const std::string& url,const std::string& method);
        Task(const std::string& url);

        virtual ~Task();

        virtual void timeOut();
        virtual void exec();

        protected:
        std::string task_tag_;
        int time_out_=60*1000;//默认超时 1分粥
        std::chrono::time_point<std::chrono::steady_clock,std::chrono::milliseconds> task_creatTime; //创建任务时间
    };


    struct ThreadPoolDelegateModelData;
    class ThreadDelegate;
    class ThreadPoolDelegateModel
    {
        friend class ThreadDelegate;
        public:
        ThreadPoolDelegateModel();
        ~ThreadPoolDelegateModel();
        //-1 启动失败
        int start(int threadnum,int max_task_num=0,int work_thread_num=0);
        //停止线程不等待任务结束
        void stop();
        //停止线程等待任务结束
        void waitAndStop();

        size_t getTaskCount() const;

        bool postTask(std::shared_ptr<Task> ptrTask);
    private:
        void run();
    private:
        std::unique_ptr<ThreadPoolDelegateModelData> p_;//
    };
}