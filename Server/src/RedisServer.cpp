#include "../include/RedisServer.h"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

static RedisServer* globalServer = nullptr;

RedisServer::RedisServer(int port) : port(port), server_socket(-1), running(true){
  globalServer = this;
}

void RedisServer::run(){
  server_socket = socket(AF_INET,SOCK_STREAM,0);

  if(server_socket < 0){
    std::cerr << "Error Creating Socket server.\n";
    return;
  }

  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_in serverAddr{};

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  if(bind(server_socket,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cerr << "Error Binding Server socket\n";
    return;
  }

  if (listen(server_socket,10) < 0){
    std::cerr << "Error Listening to Server socket\n";
    return;
  }

  std::cout << "Redis Server is listening on port "<< port <<"...\n";

  //---------------------------------------------------------------------------
  // Start the loop!
    while (running) {
        sockaddr_in clientAddr{};
        int addrlen = sizeof(clientAddr);

        // This blocks and waits for a NEW connection every time the loop restarts
        int client_socket = accept(server_socket, (struct sockaddr*)&clientAddr, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            if (running) perror("Accept failed");
            break; 
        }

        std::cout << "Connection established!\n";

        const char* hello = "Hello from the C++ Server!\n";
        send(client_socket, hello, strlen(hello), 0);

        // Close ONLY the client socket so the loop can go back to accept()
        close(client_socket);
        std::cout << "Client disconnected. Waiting for next connection...\n";
    }
  //---------------------------------------------------------------------------

  close(server_socket);

}

void RedisServer::shutdown(){
  running = false;
  if(server_socket != -1){
    close(server_socket);
  }
  std::cout << "Server Shutdown Complete!\n";

}