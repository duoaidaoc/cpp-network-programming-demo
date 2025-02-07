#pragma once

#include <cstdint>

class Epoll;

class Channel{
private:
    Epoll *ep;
    int fd;
    uint32_t events, revents;
    bool inEpoll;

public:
    Channel(int, Epoll *);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();

    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);
};
