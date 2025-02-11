#include "Channel.hpp"

#include "EventLoop.hpp"
#include <sys/epoll.h>

Channel::Channel(int _fd, EventLoop *_elp)
    :elp(_elp),fd(_fd),events(0),revents(0),inEpoll(false){
}

Channel::~Channel(){
    
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    elp->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev){
    revents = _ev;
}

void Channel::handleEvent()
{
    elp->addThread(callback);
}

void Channel::setCallback(std::function<void()>_callback)
{
    callback = _callback;
}
