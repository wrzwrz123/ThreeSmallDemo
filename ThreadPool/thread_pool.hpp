#ifndef _THREADPOOL_HPP_
#define _THREADPOOL_HPP_

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <time.h>
#include <vector>
#include <queue>
#include <future>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <condition_variable>
#include <string>
#include <shared_mutex>

using namespace std;


template<typename T>
struct safe_queue {
    queue<T>que;
    shared_mutex _m;
    bool empty() {
        shared_lock<shared_mutex> lck(_m);
        return que.empty();
    }
    auto size() {
        shared_lock<shared_mutex> lck(_m);
        return que.size();
    }
    void push(T& t) {
        unique_lock<shared_mutex> lck(_m);
        que.push(t);
    }
    bool pop(T& t) {
        unique_lock<shared_mutex> lck(_m);
        if (que.empty()) return false;
        t = move(que.front());
        que.pop();
        return true;
    }
};

class thread_pool {
  private:
    class worker {
      public:
        thread_pool* pool;

        worker(thread_pool* _pool) : pool{ _pool } {}

        void operator ()() {
            while (!pool->is_shut_down) {
                {
                    unique_lock<mutex> lock(pool->_m);
                    pool->cv.wait(lock, [this]() {
                        return this->pool->is_shut_down ||
                            !this->pool->que.empty();
                        });
                }
                function<void()>func;
                bool flag = pool->que.pop(func);
                if (flag) {
                    func();
                }
            }
        }
    };

  public:
    bool is_shut_down;
    safe_queue<std::function<void()>> que;
    vector<std::thread>threads;
    mutex _m;
    condition_variable cv;

    thread_pool(int n) : threads(n), is_shut_down{ false } {
        for (auto& t : threads)t = thread{ worker(this) };
    }

    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool& operator=(thread_pool&&) = delete;

    template <typename F, typename... Args>
    auto submit(F&& f, Args &&...args) -> std::future<decltype(f(args...))> {
        function<decltype(f(args...))()> func = [&f, args...]() {return f(args...); };
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>> (func);
        std::function<void()> warpper_func = [task_ptr]() {
            (*task_ptr)();
        };
        que.push(warpper_func);
        cv.notify_one();
        return task_ptr->get_future();
    }
    
    ~thread_pool() {
        auto f = submit([]() {});
        f.get();
        is_shut_down = true;
        cv.notify_all(); // 通知，唤醒所有工作线程
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }
};

#endif // _THREADPOOL_HPP_