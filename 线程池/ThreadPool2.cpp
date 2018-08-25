#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

using namespace std;

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;
    std::mutex mx;
    std::condition_variable condition;
    std::atomic<bool> stop;
public:
    ThreadPool(size_t);
    template<typename F, typename... Args>
            auto enqueue(F&& f, Args&&... args)
                    ->std::future<decltype(f(args...))>;
    ~ThreadPool();
};
ThreadPool::ThreadPool(size_t threads):stop(false) {
    for(size_t i=0; i<threads; ++i) {
        workers.emplace_back([this]()->void{
            while(1) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mx);
                    condition.wait(lock, [this](){return stop || !tasks.empty();});
                    if(stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}
template<typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<decltype(f(args...))>
{
    using return_type = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<return_type()> > (
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mx);
        tasks.emplace([task](){(*task)();});
    }
    condition.notify_one();
    return res;
}
ThreadPool::~ThreadPool() {
    stop.store(true);
    condition.notify_all();
    for(std::thread& worker:workers) {
        if(worker.joinable())
            worker.join();
    }
}

int main() {
    ThreadPool pool(4);
    std::vector<std::future<int> > results;
    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
                pool.enqueue([i]()->int {
                    std::cout << "hello " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << "world " << i << std::endl;
                    return i*i;
                })
        );
    }
    for(auto &result:results)
        cout << result.get() << endl;
    return 0;
}
