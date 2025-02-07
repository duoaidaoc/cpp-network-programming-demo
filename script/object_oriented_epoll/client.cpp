#include "Epoll.hpp"
#include "InetAddress.hpp"
#include "Socket.hpp"
#include <memory>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <util.hpp>

#define BUFFER_SIZE 1024

int main(){
    Socket * client = new Socket();

    uint16_t port;
    printf("Please enter the port...\n");
    scanf("%hd",&port);

    InetAddress * server_ip = new InetAddress("127.0.0.1", port);
    client->connect(server_ip);

    while(true){
        char buf[BUFFER_SIZE];  //在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(client->getFd(), buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(client->getFd(), buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(client->getFd());
            errif(true, "socket read error");
        }
    }
    close(client->getFd());
    
    return 0;
}