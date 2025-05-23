#include "../includes/ServerManager.hpp"

ServerManager::ServerManager(const ServerConfig& config) {
    for (size_t i = 0; i < config.getServersCount(); i++) {
        const ServerDirective& directive = config.getServer(i);
        try {
            Server* server = new Server(directive);
            _serversMap[server->getSocketFd()] = server;
            // std::cout << "Server created with fd: " << server->getSocketFd() << std::endl;
        } catch (std::exception& e) {
            throw Exception("Failed to initialize server " +
                            config.getServer(i).getListen()->getIp() + ":" +
                            config.getServer(i).getListen()->getPort());
        }
    }
}

void ServerManager::run() {
    for (std::map<int, Server*>::iterator it = _serversMap.begin(); it != _serversMap.end(); it++) {
        int fd = it->first;
        Server* server = it->second;

        server->start();

        struct pollfd pfd;
        pfd.fd = fd;                    // socket for our server
        pfd.events = POLLIN | POLLOUT;  // we are waiting for incoming connections
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

        for (size_t i = 0; i < _pollFds.size(); i++) {
            int fd = _pollFds[i].fd;

            Server* server = _serversMap[fd];
            Client* client = _clientsMap[fd];

            // TODO: remove this (its just for debugging)
            // std::string revents = (_pollFds[i].revents & POLLIN) ? "POLLIN" : "POLLOUT";
            // std::string type = "";
            // if (server) type = "server";
            // if (client) type = "client";
            // if (!server && !client) type = "unknown";
            // if (server && client) type = "both";
            // std::cout << "<<<<<<poll called with fd: " << fd << " and revents: " << revents << "
            // and type: " << type << ">>>>>>" << std::endl;
            // ================================

            if (server) {
                if (_pollFds[i].revents & POLLIN) {
                    int newClientFd = server->acceptClient();
                    if (newClientFd > 0) {
                        Client* newClient = new Client(newClientFd, server->getConfig());
                        _clientsMap[newClientFd] = newClient;
                        pollfd newPfd;
                        newPfd.fd = newClientFd;
                        newPfd.events = POLLIN;
                        newPfd.revents = 0;
                        _pollFds.push_back(newPfd);
                    }
                }
                continue;
            }

            if (!client) continue;

            if (_pollFds[i].revents & POLLIN) {
                ssize_t receivedDataLength = client->receive();
                if (receivedDataLength <= 0) {
                    client->close();
                    delete client;
                    _clientsMap.erase(fd);
                    _pollFds.erase(_pollFds.begin() + i);
                    i--;
                } else {
                    _pollFds[i].events = POLLOUT;
                }
            } else if (_pollFds[i].revents & POLLOUT) {
                HttpRequest request(client->getConfig(), client->getBuffer());
                if (request.getResponse().empty()) continue;
                client->send(request.getResponse());
                // std::cout << "CONNECTION WITH CLIENT CLOSED: " << client->getFd() << std::endl;
                client->close();
                delete client;
                _clientsMap.erase(fd);
                _pollFds.erase(_pollFds.begin() + i);
                i--;
            }
        }
    }
    std::cout << "ServerManager destroyed" << std::endl;
}

ServerManager::~ServerManager() {
    _pollFds.clear();

    for (std::map<int, Server*>::iterator it = _serversMap.begin(); it != _serversMap.end(); it++) {
        delete it->second;
    }
    _serversMap.clear();

    for (std::map<int, Client*>::iterator it = _clientsMap.begin(); it != _clientsMap.end(); it++) {
        delete it->second;
    }
    _clientsMap.clear();

    std::cout << "ServerManager destroyed" << std::endl;
}
