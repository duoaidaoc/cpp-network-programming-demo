#include "Server.hpp"
#include "Socket.hpp"
#include "InetAddress.hpp"
#include "Channel.hpp"
#include <functional>
#include <cstring>
#include <unistd.h>
#include <iostream>

#define READ_BUFFER 1024

Server::Server(EventLoop *_eventloop) : eventloop(_eventloop){    
    Socket *socket = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 0);
    socket->bind(serv_addr);
    socket->listen(); 
    socket->setnonblocking();
       
    Channel *channel = new Channel(socket->getFd(),eventloop);
    std::function<void()> callback = std::bind(&Server::newConnection, this, socket);
    channel->setCallback(callback);
    channel->enableReading();
}

Server::~Server()
{
    
}

void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } 
        else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } 
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } 
        else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *_socket){
    InetAddress *ip = new InetAddress();      //会发生内存泄露！没有delete
    Socket *newSocket = new Socket(_socket->accept(ip));       //会发生内存泄露！没有delete
    printf("new client fd %d! IP: %s Port: %u\n", newSocket->getFd(), ip->getIp().c_str(), ip->getPort());
    newSocket->setnonblocking();
    Channel *channel = new Channel(newSocket->getFd(),eventloop);
    std::function<void()> callback = std::bind(&Server::handleReadEvent, this, newSocket->getFd());
    channel->setCallback(callback);
    channel->enableReading();
}