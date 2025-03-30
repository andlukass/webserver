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

class Server {
   private:
    int _socketFd;
    int _port;
    std::string _ip;
	std::string _root;

    // just for the sake of orthodox form, not really needed
    Server();
    Server(const Server& other);
    Server& operator=(const Server& other);

   public:
    Server(int port, std::string ip, std::string root);
    ~Server();

    void start();
    void stop();
    int getSocketFd() const;
    void acceptClient();
};

#endif  // SERVER_HPP
