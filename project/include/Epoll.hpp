# pragma once

#include <sys/epoll.h>

#include <vector>

#include <memory>

#include "Channel.hpp"

class Epoll{
private:
    int epfd;
    epoll_event *events;

public: 
    void addFd(int fd, uint32_t op);
    Epoll();
    ~Epoll();

    void updateChannel(Channel*);

    std::unique_ptr<std::vector<Channel>>poll(int timeout = -1);
    //std::unique_ptr<std::vector<epoll_event>>poll(int timeout = -1);

};