#pragma once

#include<map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server
{
private:
    EventLoop *eventloop;
    Acceptor* acceptor;
    std::map<int,Connection*>connections;

public:
    Server();
    ~Server();

    // void handleReadEvent(int);
    void newConnection(Socket*);
    void deleteConnection(Socket*);
    void start();
};