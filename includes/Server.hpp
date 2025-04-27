#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <sstream>
#include "./directives/ServerDirective.hpp"
#include "./HttpRequest.hpp"

class Server {
   private:
    int _socketFd;
    const ServerDirective& _config;

    // just for the sake of orthodox form, not really needed
    Server();
    Server(const Server& other);
    Server& operator=(const Server& other);

   public:
   Server(const ServerDirective& config);
    ~Server();

    void start();
    void stop();
    int getSocketFd() const;
    int acceptClient();
    const ServerDirective& getConfig() const;
};

#endif  // SERVER_HPP
