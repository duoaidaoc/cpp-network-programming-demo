#pragma once

#include <cstdint>
#include <functional>

class Epoll;
class EventLoop;

class Channel{
private:
    EventLoop *elp;
    int fd;
    uint32_t events, revents;
    bool inEpoll;
    std::function<void()> callback;

public:
    Channel(int, EventLoop *);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();

    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);
    void handleEvent();
    void setCallback(std::function<void()>);
};
