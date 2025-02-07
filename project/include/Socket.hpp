#pragma once

#include "InetAddress.hpp"

class Socket{
private:
    int fd; 
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress* );
    void listen();
    void setnonblocking();
    int accept(InetAddress* );
    int getFd();
    void connect(InetAddress* );
};