#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

#define MAX_EVENTS 10
#define PORT 8080

// 设置文件描述符为非阻塞模式
int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL failed");
        return -1;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
        return -1;
    }
    return 0;
}

// 处理客户端数据
void handle_client(int client_fd) {
    char buffer[1024];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Read failed");
    } else if (bytes_read == 0) {
        std::cout << "Client disconnected." << std::endl;
    } else {
        buffer[bytes_read] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;
        // Echo the data back to the client
        write(client_fd, buffer, bytes_read);
    }
}

int main() {
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    struct epoll_event ev, events[MAX_EVENTS];

    // 创建服务器 socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return -1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定服务器地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        return -1;
    }

    // 监听端口
    if (listen(server_fd, 5) == -1) {
        perror("listen failed");
        return -1;
    }

    // 设置 server_fd 为非阻塞
    if (set_nonblocking(server_fd) == -1) {
        return -1;
    }

    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        return -1;
    }

    // 将 server_fd 添加到 epoll 实例中，监听可读事件
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl failed");
        return -1;
    }

    // 事件循环
    while (true) {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_fd) {
                // 新的客户端连接
                client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd == -1) {
                    perror("accept failed");
                    continue;
                }

                std::cout << "New client connected." << std::endl;

                // 设置客户端 socket 为非阻塞
                if (set_nonblocking(client_fd) == -1) {
                    close(client_fd);
                    continue;
                }

                // 将 client_fd 添加到 epoll 实例中，监听可读事件
                ev.events = EPOLLIN | EPOLLET; // 设置边缘触发模式
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl failed");
                    close(client_fd);
                }
            } else {
                // 处理客户端数据
                handle_client(events[i].data.fd);
            }
        }
    }

    // 关闭文件描述符
    close(server_fd);
    close(epoll_fd);
    return 0;
}
