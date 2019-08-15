#pragma once

#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <vector>
#include "range.h"
#include <queue>
#include <assert.h>

namespace sy
{
class ThreadPool
{
public:
    ThreadPool(int size)
        :stop(false)
    {
        for(auto i = 0; i < size; i ++ )
        {
            workers.emplace_back([this]{
                    while(true)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            condition.wait(lock, [this]{return stop || !tasks.empty(); });
                            if(stop && tasks.empty())
                            {
                                return;
                            }
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                    });
        }
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    template<typename Fn, typename ...Arg>
    auto run(Fn&& fn, Arg&& ...arg) -> std::future<typename std::result_of<Fn(Arg ...)>::type>
    {
        using task_type = typename std::result_of<Fn(Arg ...)>::type;
        auto task = std::make_shared<std::packaged_task<task_type()>>(std::bind(std::forward<Fn>(fn), std::forward<Arg>(arg) ...));
        auto ret = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx);
            assert(!stop);
            tasks.push([task]{
                    (*task)();
                    });
        }
        condition.notify_one();
        return ret;
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }
        condition.notify_all();
        for(auto &th : workers)
        {
            th.join();
        }
    }
private:
    std::mutex mtx;
    std::condition_variable condition;
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> workers;
    bool stop;
}; // class ThreadPool
} //namespace sy
