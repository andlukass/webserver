#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

class Server {
   private:
    int _socketFd;
    int _port;
    std::string _ip;

    // just for the sake of orthodox form, not really needed
    Server();
    Server(const Server& other);
    Server& operator=(const Server& other);
    void acceptClient();

   public:
    Server(int port, std::string ip);
    ~Server();

    void start();
    void stop();
};

#endif  // SERVER_HPP
