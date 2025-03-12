#include "EventLoop.hpp"
#include "Server.hpp"

#define BUFFER_SIZE 1024 


int main() {
    Server *server = new Server();
    delete server;
    return 0;
}