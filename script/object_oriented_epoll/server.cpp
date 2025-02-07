#include "Epoll.hpp"
#include "InetAddress.hpp"
#include "Socket.hpp"
#include <memory>
#include <iostream>
#include <cstring>
#include <unistd.h>

#define READ_BUFFER 1024

void handleReadEvent(int);

int main(){
    Socket* server = new Socket();
    InetAddress* ip = new InetAddress("127.0.0.1",0);
    server->bind(ip);
    
    Epoll* ep = new Epoll();
    ep->addFd(server->getFd(), EPOLLIN | EPOLLET);

    server->listen();
    server->setnonblocking();

    while(true){
        auto channels = ep->poll();
        for(auto channel: *channels){
            if(channel.getFd() == server->getFd()){
                InetAddress *client_ip = new InetAddress();
                Socket *client = new Socket(server->accept(client_ip));
                printf("new client fd %d! Ip:%s Port:%d\n",client->getFd(),inet_ntoa(client_ip->addr.sin_addr),client_ip->addr.sin_port);
                client->setnonblocking();
                ep->addFd(client->getFd(), EPOLLIN | EPOLLET);
            }
            else if(channel.getEvents() & EPOLLIN){
                printf("handling data from %d\n", channel.getFd());
            }
            else{
                
                // ...
            }
        }
    }
    delete server;
    delete ip;
    
    return 0;
}

void handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while(true){
        memset(buf,0,sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf,sizeof(buf));
        if(bytes_read > 0){
            printf("message frome client fd %d: %s.\n",sockfd, buf);
        }
        else if(bytes_read == -1 && errno == EINTR){
            printf("continue reading.\n");
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
