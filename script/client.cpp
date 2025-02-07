#include <sys/socket.h> // linux 独有

#include <arpa/inet.h> 

#include <cstring>

#include <iostream>



int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // SOCK_STREAM -> TCP
    // SOCK_DGRAM -> UDP

    // struct sockaddr_in self_addr;
    // bzero(&self_addr, sizeof(self_addr));
    // self_addr.sin_family = AF_INET;
    // self_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 本地绑定ip
    // self_addr.sin_port = 0; // 本地端口自由选择

    // bind(sockfd, (sockaddr *)&self_addr, sizeof(sockaddr_in));
    // // 端口绑套接字文件

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 将字符串形式的点分十进制ip转化为int
    serv_addr.sin_port = htons(8888); // 端口为0操作系统会自动分配端口

    // 客户将serv_addr.sin_addr.s_addr 设置为 服务器ip

    // 套接字文件连接服务器。
    int cnt_id = connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));

    if(cnt_id >= 0)
        printf("server connected.");
    
    return 0;

}