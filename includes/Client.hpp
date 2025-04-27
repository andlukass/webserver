#pragma once

#include <sys/socket.h>
#include <unistd.h> 
#include <string>
#include <iostream>
class Client {
    private:
        int _fd;
        char _buffer[1024];

    public:
        Client(int fd);
        ~Client();

        void send(const std::string& response);
        void close();
        void receive();

        std::string getBuffer() const;
};