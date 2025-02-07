#include "Socket.hpp"
#include "util.hpp"

#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>


Socket::Socket() : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "Failed when creating socket");
}

Socket::Socket(int _fd) : fd(_fd)
{
    errif(fd == -1, "Failed when creating socket");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr){
    if(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1)
        printf("Failed when binding.\n");
    else{
        getsockname(fd, (struct sockaddr *)&addr->addr, &addr->addr_len);
        printf("Socket binds on port %d.\n", addr->getPort());
    }
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "Failed when listening");
}

void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&(addr->addr), &addr->addr_len);
    errif(clnt_sockfd == -1, "Failed when accepting");
    return clnt_sockfd;
}

int Socket::getFd(){
    return fd;
}

void Socket::connect(InetAddress * addr)
{
    errif(::connect(fd, (sockaddr *)&addr->addr, addr->addr_len),"Failed when Connecting to server.");
}
