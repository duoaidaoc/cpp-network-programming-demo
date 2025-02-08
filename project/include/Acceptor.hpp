#pragma once

#include<functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor{
private:
    EventLoop* eventloop;
    Socket* socket;
    InetAddress* ip;
    Channel *acceptChannel;
public:
    Acceptor(EventLoop*);
    ~Acceptor();
    void acceptConnection();

    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};