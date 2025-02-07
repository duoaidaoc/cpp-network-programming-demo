#pragma once

class EventLoop;
class Socket;
class Server
{
private:
    EventLoop *eventloop;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket*);
};