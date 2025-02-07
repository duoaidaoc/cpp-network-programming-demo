#include "Epoll.hpp"


#include "util.hpp"
#include <cstring>
#include <unistd.h>


#define MAX_EVENTS 1000

Epoll::Epoll():epfd(-1),events(nullptr)
{
    epfd = epoll_create1(EPOLL_CLOEXEC);
    errif(epfd == -1, "Failed when creating epoll instance.");
    events = new epoll_event[MAX_EVENTS];
    memset(events, 0, sizeof(events));
}

Epoll::~Epoll()
{
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete []events;
}

void Epoll::updateChannel(Channel *channel)
{
    int fd = channel->getFd();
    epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        errif(
            epoll_ctl(epfd,EPOLL_CTL_ADD,channel->getFd(),&ev) == -1, 
            "Failed when adding to epoll."
        );
        channel->setInEpoll();
    } 
    else{
        errif(
            epoll_ctl(epfd, EPOLL_CTL_MOD,channel->getFd(),&ev),
            "Failed when modifying an existing event."
        );
    }
}

void Epoll::addFd(int fd, uint32_t op)
{
    // op 表示
    /*
        EPOLLIN 可读
        EPOLLOUT 可写
        EPOLLERR 错误
        EPOLLHUP 对侧是否关闭连接
        EPOLLET 边缘触发
    */
    epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "Failed when adding event into epoll.");
}

// std::unique_ptr<std::vector<epoll_event>> Epoll::poll(int timeout)
// {
//     auto activeEvents = std::make_unique<std::vector<epoll_event> >();
//     int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
//     // errif(nfds == -1, "epoll wait error.");
//     for(int i = 0; i < nfds; i++){
//         activeEvents->push_back(events[i]);
//     }
//     return activeEvents;
// }

std::unique_ptr<std::vector<Channel>> Epoll::poll(int timeout)
{
    auto activeEvents = std::make_unique<std::vector<Channel> >();
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    // errif(nfds == -1, "epoll wait error.");
    for(int i = 0; i < nfds; i++){
        Channel channel(events[i].data.fd, this);
        activeEvents->push_back(channel);
    }
    return activeEvents;
}