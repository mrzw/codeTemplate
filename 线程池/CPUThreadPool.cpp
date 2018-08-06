#include <iostream>
#include <functional>
#include <atomic>
#include <vector>
#include <thread>
#include <queue>
#include <future>
#include <algorithm>
#include <condition_variable>

using namespace std;

class TaskExecutor {
    using Task = std::function<void()>;
private:
    //线程池
    std::vector<std::thread> pool_;
    //任务队列
    std::queue<Task> tasks_;
    //是否关闭提交
    std::atomic_bool stop_;
    //线程池大小
    size_t size_;
    //同步
    std::mutex mu_;
    //条件变量
    std::condition_variable cv_task_;
    //空闲线程数量
    std::atomic_int idlThreadNum_;
public:
    TaskExecutor(size_t s=0):size_(s),stop_(false) {
        if(size_ == 0)
            size_ = std::thread::hardware_concurrency(); //默认线程池大小等于CPU核数
        idlThreadNum_ = size_;
        //初始化工作线程数量
        for(int i=0; i<size_; ++i) {
            pool_.emplace_back([this]()
                               {
                                   while(!this->stop_) {
                                       --this->idlThreadNum_;
                                       this->get_one_task()();
                                       ++this->idlThreadNum_;
                                   }
                                   cout<<"线程 id="<<std::this_thread::get_id()<<" 退出！"<<endl;
                               });
        }
    }
    ~TaskExecutor()
    {
        stop_.store(true);
        cv_task_.notify_all();
        for(std::thread& t:pool_)
        {
            if(t.joinable())
                t.join();
        }
    }
    //停止任务的提交
    void shutdown()
    {
        this->stop_.store(true);
    }
    //重启任务提交
    void restart()
    {
        this->stop_.store(false);
    }
    //获取空闲线程数量
    int getIdlCount()
    {
        return idlThreadNum_;
    }
    //提交一个任务
    template <typename F, typename ... Args>
    auto commit(F&& f, Args&& ... args) -> std::future<decltype(f(args...))> {
        if(stop_.load())
        {
            throw std::runtime_error("task executor closed commit!!!");
        }
        using ResType = decltype(f(args...)); // 函数ｆ的返回值类型
        auto task = std::make_shared<std::packaged_task<ResType()>> (
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        std::future<ResType> future = task->get_future();
        {
            //添加任务到任务队列，需要加锁
            std::lock_guard<std::mutex> lock_append(mu_);
            tasks_.emplace(
                    [task]()
                    {
                        (*task)();
                    });
        }
        //条件变量唤醒一个线程去执行任务
        cv_task_.notify_one();
        //返回异步执行结果
        return future;
    };
private:
    //获取一个待执行的task
    Task get_one_task() {
        Task task;
        std::unique_lock<std::mutex> lock(mu_);
        //条件变量wait()直到有任务队列中有数据
        this->cv_task_.wait(lock, [this]() -> bool
        {
            return this->stop_.load() || !this->tasks_.empty();
        });
        //取一个任务
        task = std::move(this->tasks_.front());
        this->tasks_.pop();
        return std::move(task);
    }
};

void task1(int x)
{
    cout<<"线程id="<<this_thread::get_id()<<"输出："<<x<<endl;
}

int main() {
    TaskExecutor taskExecutor;
    cout << "空闲线程数量：" << taskExecutor.getIdlCount() << endl;

    for (int i = 0; i < 10; i++) {
        std::future<void> a = taskExecutor.commit(&task1, i);
    }

    while(1)
    {
        this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
