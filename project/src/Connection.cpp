#include "Connection.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include "util.hpp"
#include "Buffer.hpp"

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
    readBuf = new Buffer();
}

Connection::~Connection()
{
    delete channel;
    delete socket;
    delete readBuf;
}

void Connection::echo(int sockfd)
{
    char buf[1024];     //这个buf大小无所谓
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            readBuf->append(buf, bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once\n");
            printf("message from client fd %d: %s\n", sockfd, readBuf->c_str());
            errif(write(sockfd, readBuf->c_str(), readBuf->size()) == -1, "socket write error");
            readBuf->clear();
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(socket);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _deleteConnectionCallback)
{
    deleteConnectionCallback = _deleteConnectionCallback;
}
