#include "ThreadPool.hpp"
#include <stdexcept>

ThreadPool::ThreadPool(int size) : stop(false)
{
    for(int i = 0; i< size; ++i){
        threads.emplace_back(std::thread([this](){
            while(true){
                std::function<void()>task;
                {
                    std::unique_lock<std::mutex>lock(tasks_mtx); 
                    cv.wait(lock, [this](){
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex>lock(tasks_mtx);
        stop = 1;
    }
    for(auto &th : threads){
        if(th.joinable()){
            th.join();
        }
    }
    
}

void ThreadPool::add(std::function<void()>func)
{
    { //在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if(stop)
            throw std::runtime_error("ThreadPool already stop, can't add task any more");
        tasks.emplace(func);
    }
    cv.notify_one();    //通知一次条件变量
}
