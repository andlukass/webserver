#include "../includes/ServerManager.hpp"

#include <poll.h>

ServerManager::ServerManager(const ServerConfig& config) {
    std::string ip;
    int port;

    for (size_t i = 0; i < config.getServersCount(); i++) {
        ip = config.getServer(i).getListen()->getIp();
        port = config.getServer(i).getListen()->getPortInt();
        // if this causes problems, we can switch to std::vector<Server*>
        try {
            _servers.push_back(new Server(port, ip));
        } catch (std::exception& e) {
            throw Exception("Failed to initialize server " + ip + ":" +
                            config.getServer(i).getListen()->getPort());
        }
    }
}

void ServerManager::run() {
    for (size_t i = 0; i < _servers.size(); i++) {
        int fd = _servers[i]->getSocketFd();

        _servers[i]->start();

        struct pollfd pfd;
        pfd.fd = fd;          // socket for our server
        pfd.events = POLLIN;  // we are waiting for incoming connections
        pfd.revents = 0;  // system will fill it later, once some event happens (incoming message)

        _pollFds.push_back(pfd);
    }

    while (true) {
        // we need to refer to the beginning of _pollFds vector to monitor all of them
        int ready = poll(&_pollFds[0], _pollFds.size(), -1);  // -1 means wait forever

        if (ready < 0) {
            std::cerr << "Error: poll() failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                _servers[i]->acceptClient();
            }
        }
    }
}

ServerManager::~ServerManager() {
    _pollFds.clear();
    for (size_t i = 0; i < _servers.size(); ++i) {
        delete _servers[i];
    }
    std::cout << "ServerManager destroyed" << std::endl;
}
