#pragma once

class EventLoop;
class Socket;
class Server;

class Acceptor{
private:
    EventLoop* eventloop;
    Socket* socket;
    Server* server;
public:
    Acceptor(EventLoop*, Socket*, Server*);
    void newConnection(Socket*);
};