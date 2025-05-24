#include "../includes/Server.hpp"

#include "../includes/Client.hpp"
#include "../includes/Exception.hpp"

Server::Server(const ServerDirective& config) : _config(config) {
    std::cout << "Creating server with IP: " << config.getListen()->getIp()
              << " and port: " << config.getListen()->getPort() << std::endl;

    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        //std::cerr << "Error: Can not create socket" << std::endl;
        throw Exception("Error: Can not create socket");
    }
    std::cout << "Socket created successfully!" << std::endl;
}

Server::~Server() {
    if (_socketFd > -1) {
        close(_socketFd);
        _socketFd = -1;
    }
}

int Server::acceptClient() {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    int clientFd = accept(_socketFd, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        std::cerr << "Error: Cannot accept client connection" << std::endl;
    } else {
        std::cout << "Client connected!" << std::endl;
    }
    return clientFd;
}

void Server::start() {
    // Check if the socket is already closed or invalid
    if (_socketFd == -1) {
        throw Exception("Error: Server already stopped");
    }

    // allow reusing port to avoid "Address already in use" error
    int opt = 1;
    setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Set up hints for getaddrinfo to request IPv4 and TCP socket
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // allows even IP 0.0.0.0

    struct addrinfo* res;
    // getaddrinfo converts IP and port string into a usable sockaddr structure
    // c_str returns pointer to first element of the internal string buffer
    int addrInfoError = getaddrinfo(_config.getListen()->getIp().c_str(),
                                    _config.getListen()->getPort().c_str(), &hints, &res);
    if (addrInfoError) {
        // gai_strerror gets error message from getaddrinfo
        std::cerr << "Error: getaddrinfo failed: " << gai_strerror(addrInfoError) << std::endl;
        throw Exception("Error: getaddrinfo failed");
    }

    // std::cout << "Binding!" << std::endl;

    // Bind socket to resolved address (IP and port)
    if (bind(_socketFd, res->ai_addr, res->ai_addrlen) < 0) {
        //std::cerr << "Error: Cannot bind socket" << std::endl;
        freeaddrinfo(res);
        throw Exception("Error: Cannot bind socket");
    }

    std::cout << "Binding successful!" << std::endl;

    freeaddrinfo(res);

    // Start listening on the socket with a queue size of 10
    if (listen(_socketFd, 10) < 0) {
        throw Exception("Error: listen failed");
    }
    std::cout << "Server started (for real!) and listening on port "
              << _config.getListen()->getPort() << std::endl;
}

void Server::stop() {
    if (_socketFd > -1) {
        close(_socketFd);
        _socketFd = -1;
    }
}

int Server::getSocketFd() const { return _socketFd; }

const ServerDirective& Server::getConfig() const { return _config; }
