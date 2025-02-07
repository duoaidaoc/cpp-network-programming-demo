#include "Acceptor.hpp"

#include "Channel.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "InetAddress.hpp"

#include <functional>
#include <unistd.h>


Acceptor::Acceptor(EventLoop* _eventloop, Socket* _socket, Server* _server): eventloop(_eventloop),socket(_socket),server(_server)
{
    Channel *channel = new Channel(socket->getFd(),eventloop);
    std::function<void()> callback = std::bind(&Acceptor::newConnection, this, socket);
    channel->setCallback(callback);
    channel->enableReading();
}

void Acceptor::newConnection(Socket* _socket)
{
    InetAddress *ip = new InetAddress();      //会发生内存泄露！没有delete
    Socket *newSocket = new Socket(_socket->accept(ip));       //会发生内存泄露！没有delete
    printf("new client fd %d! IP: %s Port: %u\n", newSocket->getFd(), ip->getIp().c_str(), ip->getPort());
    newSocket->setnonblocking();
    Channel *channel = new Channel(newSocket->getFd(),eventloop);
    std::function<void()> callback = std::bind(&Server::handleReadEvent, server, newSocket->getFd());
    channel->setCallback(callback);
    channel->enableReading();
}
