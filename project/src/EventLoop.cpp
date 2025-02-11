#include "EventLoop.hpp"
#include "Epoll.hpp"
#include "Channel.hpp"
#include "ThreadPool.hpp"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false), threadPool(nullptr){
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep;
}

void EventLoop::loop(){
    while(!quit){
    std::vector<Channel*> chs;
        auto channels = *ep->poll();
        for(auto& channel : channels){
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *channel){
    ep->updateChannel(channel);
}

void EventLoop::addThread(std::function<void()> func){
    threadPool->add(func);
}