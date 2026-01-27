#include <iostream>
#include <sys/socket.h> // Core socket functions
#include <netinet/in.h> // Internet address family (sockaddr_in)
#include <unistd.h>      // close() function
#include <cstring>     // memset

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* hello = "Hello from the C++ Server!";

    // 1. Create Socket (The Phone Jack)
    // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set Socket Options (Allow Port Reuse)
    // This prevents the "Address already in use" error on restart
    // This 4 line is optional but recommended for development
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // 3. Define the Address (The Phone Number)
    address.sin_family = AF_INET;          // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces not a specific one
    address.sin_port = htons(8080);       // Convert port 8080 to network byte order

    // 4. Bind (Assign the address to the socket)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 5. Listen (Wait for the ringer)
    // 3 is the backlog (queue size)
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    // 6. Accept (Pick up the phone)
    // This blocks the program until a client connects
    // address will be populated with the client's address info
    // addrlen is a value-result parameter
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Connection established!" << std::endl;

    // 7. Communication (Send/Receive)
    // Send a message to the client
    // This will be introduced in the next updates
    send(client_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent to client." << std::endl;

    // 8. Close the connections
    close(client_socket); // Hang up the current call
    close(server_fd);    // Unplug the phone jack

    return 0;
}