#pragma once
#include <functional>

class EventLoop;
class Channel;
class Socket;


class Connection{
private:
    EventLoop* elp;
    Socket* socket;
    Channel* channel;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(EventLoop*,Socket*);
    ~Connection();
    void echo(int);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};