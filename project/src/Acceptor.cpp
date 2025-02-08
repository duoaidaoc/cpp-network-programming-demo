#include "Acceptor.hpp"

#include "Channel.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "InetAddress.hpp"

#include <functional>
#include <unistd.h>


Acceptor::Acceptor(EventLoop* _eventloop): eventloop(_eventloop)
{
    
    socket = new Socket();
    ip = new InetAddress("127.0.0.1", 0);
    socket->bind(ip);
    socket->listen(); 
    socket->setnonblocking();
    acceptChannel = new Channel(socket->getFd(), eventloop);
    // 这里不能直接用c++ functional? 只能包装一下？
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor()
{
    delete socket;
    delete ip;
}

void Acceptor::acceptConnection()
{
    newConnectionCallback(socket);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)>_cb)
{
    newConnectionCallback = _cb;
}