#include "../includes/Server.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <fstream>  // Ensure this is included for ifstream (to open file)
#include <sstream>

// TINA: change this instead of getting port and ip just get whole config. change it also in .h -
// have _config instead of _ip and _port
Server::Server(int port, std::string ip) {
    // TODO: closer to the end of the project we can define, if _port and _ip should be const
    std::cout << "Creating server with IP: " << ip << " and port: " << port << std::endl;
    _port = port;
    _ip = ip;
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        std::cerr << "Error: Can not create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created successfully!" << std::endl;
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

    // Read the HTML file
    // TINA: use getter
    std::ifstream file("./src/webcontent/webcontent.html", std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open HTML file" << std::endl;
        close(clientFd);
        return;
    }
    std::string htmlContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    // Build HTTP response header
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";

    // Convert size to string using stringstream (for C++98)
    std::stringstream ss;
    ss << htmlContent.size();
    std::string contentLength = ss.str();
    response += "Content-Length: " + contentLength + "\r\n";

    response += "Connection: close\r\n\r\n";  // Close connection after response
    response += htmlContent;

    // Send the response to the client
    send(clientFd, response.c_str(), response.size(), 0);

    std::cout << "HTML content sent to client!" << std::endl;

    // Close the client connection
    close(clientFd);
}

void Server::start() {
    // Check if the socket is already closed or invalid
    if (_socketFd == -1) {
        std::cerr << "Error: Server already stopped" << std::endl;
        exit(EXIT_FAILURE);
    }

    // allow reusing port to avoid "Address already in use" error
    int opt = 1;
    setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Convert _port (int) to string for getaddrinfo
    std::stringstream portStream;
    portStream << _port;
    std::string portStr = portStream.str();

    // Set up hints for getaddrinfo to request IPv4 and TCP socket
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // allows even IP 0.0.0.0

    struct addrinfo* res;
    // getaddrinfo converts IP and port string into a usable sockaddr structure
    // c_str returns pointer to first element of the internal string buffer
    int status = getaddrinfo(_ip.c_str(), portStr.c_str(), &hints, &res);
    if (status != 0) {
        // gai_strerror gets error message from getaddrinfo
        std::cerr << "Error: getaddrinfo failed: " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Binding!" << std::endl;

    // Bind socket to resolved address (IP and port)
    if (bind(_socketFd, res->ai_addr, res->ai_addrlen) < 0) {
        std::cerr << "Error: Cannot bind socket" << std::endl;
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    std::cout << "Binding successful!" << std::endl;

    freeaddrinfo(res);

    // Start listening on the socket with a queue size of 10
    if (listen(_socketFd, 10) < 0) {
        std::cerr << "Error: listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started (for real!) and listening on port " << _port << std::endl;
}

void Server::stop() {
    if (_socketFd > -1) {
        close(_socketFd);
        _socketFd = -1;
    }
}

int Server::getSocketFd() const { return _socketFd; }