#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "./directives/ServerDirective.hpp"

class Client {
   private:
    int _fd;
    std::string _buffer;
    const ServerDirective& _serverConfig;

   public:
    Client(int fd, const ServerDirective& serverConfig);
    ~Client();

    ssize_t send(const std::string& response);
    ssize_t receive();
    void close();

    std::string getBuffer() const;
    const ServerDirective& getConfig() const;
    int getFd() const;
};

#endif