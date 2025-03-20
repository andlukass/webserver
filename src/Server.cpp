#include "../includes/Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

Server::Server() {
    _port = 8080;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        std::cerr << "Error: Can not create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    if (_socketFd > -1) close(_socketFd);
}

void Server::acceptClient() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_socketFd, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        std::cerr << "Error: Cannot accept client connection" << std::endl;
        return;
    }

    std::cout << "Client connected!" << std::endl;

    close(clientFd);
}

void Server::start() {
    if (_socketFd == -1) {
        std::cerr << "Error: Server already stopped" << std::endl;
        exit(EXIT_FAILURE);
    }

    // allow reusing port to avoid "Address already in use" error
    int opt = 1;
    setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(_port);

    if (bind(_socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Cannot bind socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(_socketFd, 10) < 0) {
        std::cerr << "Error: listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started (for real!) and listening on port " << _port << std::endl;

    while (true) {
        acceptClient();
    }
}

void Server::stop() {
    if (_socketFd > -1) {
        close(_socketFd);
        _socketFd = -1;
    }
}