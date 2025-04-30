#pragma once

#include <sys/socket.h>
#include <unistd.h> 
#include <string>
#include <iostream>
#include "./directives/ServerDirective.hpp"

class Client {
    private:
        int _fd;
        char _buffer[200000];
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