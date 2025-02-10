#include "Connection.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _elp,Socket* _socket)
:elp(_elp),socket(_socket),channel(nullptr)
{
    channel = new Channel(socket->getFd(),elp);
    auto cb = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection()
{
    delete channel;
    delete socket;
}

void Connection::echo(int sockfd)
{
    char buf[READ_BUFFER];
    while(true){
        memset(buf,0,sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(socket);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _deleteConnectionCallback)
{
    deleteConnectionCallback = _deleteConnectionCallback;
}
