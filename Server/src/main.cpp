#include <iostream>

#include "../include/RedisServer.h"
int main(int argc, char* argv[]) {
    int port = 6379; // default port
    if(argc > 1) port = std::stoi(argv[1]);
    
    RedisServer server(port);
    server.run();
    
    return 0;
}
