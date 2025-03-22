#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <vector>

#include "Server.hpp"
#include "ServerConfig.hpp"

class ServerManager {
   private:
    std::vector<Server*> _servers;
    std::vector<struct pollfd> _pollFds;

   public:
    ServerManager(const ServerConfig& config);
    ~ServerManager();

    void run();
};

#endif