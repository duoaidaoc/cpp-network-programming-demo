#include <sys/socket.h> // linux 独有

#include <arpa/inet.h> 

#include <cstring>

#include <iostream>

void serve(const int &sockfd){
    struct sockaddr_in clnt_addr; //为客户端创建 socket 地址信息

    socklen_t clnt_addr_len = sizeof(clnt_addr);

    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);

    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
}


int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // SOCK_STREAM -> TCP
    // SOCK_DGRAM -> UDP

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 将字符串形式的点分十进制ip转化为int
    serv_addr.sin_port = htons(8888); // 将大端序转换为小端序

    bind(sockfd, (sockaddr *)&serv_addr, sizeof(sockaddr_in));
    // linux 将套接字视为文件， 这句话将套接字文件和刚刚设定好的服务器地址进行绑定。

    listen(sockfd, SOMAXCONN);
    // 监听socket端口

    serve(sockfd);



}