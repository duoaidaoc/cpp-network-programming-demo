#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress{
public:
    sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();

    uint16_t getPort();
    std::string getIp();
};