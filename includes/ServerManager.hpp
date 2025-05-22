#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <poll.h>

#include <vector>

#include "Client.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"

class ServerManager {
   private:
    std::map<int, Server*> _serversMap;
    std::map<int, Client*> _clientsMap;
    std::vector<struct pollfd> _pollFds;

   public:
    ServerManager(const ServerConfig& config);
    ~ServerManager();

    void run();
};

#endif