#include "EventLoop.hpp"
#include "Epoll.hpp"
#include "Channel.hpp"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false){
    ep = new Epoll();
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