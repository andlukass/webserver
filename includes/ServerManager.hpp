#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <vector>
#include <poll.h>

#include "Server.hpp"
#include "Client.hpp"
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