#pragma once
#include <iostream>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

#include <chrono>

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize) : running(true) {
        for (size_t i = 0; i < poolSize; ++i) {
            workers.emplace_back([this]() {
                for(;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mutex);
                        this->cond_var.wait(lock, [this](){
                            return !(this->running && this->tasks_queue.empty());
                        });
                        if (!this->running && this->tasks_queue.empty()) {
                            return;
                        }
                        task = std::move(this->tasks_queue.front());
                        this->tasks_queue.pop();
                    }
                    task();
                }
            });
        }
    }

     // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        using return_type = typename std::result_of<Func(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(func, args...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex);

            if(!running) {
                throw std::runtime_error("ThreadPool is stopped");
            }
            tasks_queue.emplace([task](){ (*task)(); });
        }
        cond_var.notify_one();
        return res;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock{mutex};
            running = false;
        }
        cond_var.notify_all();
        for (auto & worker : workers) {
            worker.join();
        }
    }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks_queue;

    std::mutex mutex;
    std::condition_variable cond_var;
    bool running;
};