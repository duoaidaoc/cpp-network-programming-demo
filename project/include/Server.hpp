#pragma once

class EventLoop;
class Socket;
class Acceptor;

class Server
{
private:
    EventLoop *eventloop;
    Acceptor* acceptor;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket*);
};