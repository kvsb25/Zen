#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <winsock2.h>
#include "../tcp/clientSession.hpp"
#include "../tcp/utils.hpp"
#include "../app/app.hpp"

// create a generic task/function pool, don't tie to HTTP
class ThreadPool{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void(void)>> tasks;
    
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    template <class F>
    inline void enqueue(F&& f){
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    };
};

// Workers is the set (or array) of threads that handle client sessions for http requests
// Connections is the array of clients accepted by the TCP server, hence the client sessions (the sockets)