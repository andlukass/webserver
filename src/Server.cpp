#include "../includes/Server.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <sstream>

Server::Server(int port, std::string ip) {
    // TODO: closer to the end of the project we can define, if _port and _ip should be const
    _port = port;
    _ip = ip;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        std::cerr << "Error: Can not create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    if (_socketFd > -1) {
        close(_socketFd);
        _socketFd = -1;
    }
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

    std::stringstream portStream;
    portStream << _port;
    std::string portStr = portStream.str();

    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // allows even IP 0.0.0.0

    struct addrinfo* res;
    // c_str returns pointer to first element of the internal string buffer
    int status = getaddrinfo(_ip.c_str(), portStr.c_str(), &hints, &res);
    if (status != 0) {
        // gai_strerror gets error message from getaddrinfo
        std::cerr << "Error: getaddrinfo failed: " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(_socketFd, res->ai_addr, res->ai_addrlen) < 0) {
        std::cerr << "Error: Cannot bind socket" << std::endl;
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

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