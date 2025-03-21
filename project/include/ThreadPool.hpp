#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

class ThreadPool{
private:
    std::vector<std::thread>threads;
    std::queue<std::function<void()>>tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();
    void add(std::function<void ()>);
};
