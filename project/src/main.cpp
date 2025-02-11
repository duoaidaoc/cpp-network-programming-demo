#include "EventLoop.hpp"
#include "Server.hpp"

#define BUFFER_SIZE 1024 


int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    delete server;
    delete loop;
    return 0;
}