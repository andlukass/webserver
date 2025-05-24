#include "../includes/ServerManager.hpp"

ServerManager::ServerManager(const ServerConfig& config) {
    for (size_t i = 0; i < config.getServersCount(); i++) {
        const ServerDirective& directive = config.getServer(i);
        try {
            // Create a new Server using its configuration directive
            Server* server = new Server(directive);
            // Add new server to a map: key = socket FD, value = Server pointer
            _serversMap[server->getSocketFd()] = server;
        } catch (std::exception& e) {
            // If construction fails, throw a custom exception with IP and port info
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
            throw Exception("Error: poll() failed");
        }

        // Iterate through all monitored file descriptors
        for (size_t i = 0; i < _pollFds.size(); i++) {
            int fd = _pollFds[i].fd;

            Server* server = _serversMap[fd];  // Check if fd belongs to a server
            Client* client = _clientsMap[fd];  // Or to a client

            // Accept new client if fd belongs to a server and is ready for reading
            if (server) {
                if (_pollFds[i].revents & POLLIN) {
                    int newClientFd = server->acceptClient();
                    if (newClientFd > 0) {
                        Client* newClient = new Client(newClientFd, server->getConfig());
                        _clientsMap[newClientFd] = newClient;

                        // Add new client to poll monitoring
                        pollfd newPfd;
                        newPfd.fd = newClientFd;
                        newPfd.events = POLLIN;
                        newPfd.revents = 0;
                        _pollFds.push_back(newPfd);
                    }
                }
                continue;  // Do not process further if it's a server socket
            }

            // If it's not a client - just skip all the rest
            if (!client) continue;

            // If client socket is ready to read
            if (_pollFds[i].revents & POLLIN) {
                // Only one recv per client per poll()
                ssize_t receivedDataLength = client->receive();
                if (receivedDataLength <= 0) {
                    // Cleanup
                    client->close();
                    delete client;
                    _clientsMap.erase(fd);
                    _pollFds.erase(_pollFds.begin() + i);
                    i--;
                } else {
                    // Ready to send a response next time
                    _pollFds[i].events = POLLOUT;
                }
                // If client socket is ready to write
            } else if (_pollFds[i].revents & POLLOUT) {
                HttpRequest request(client->getConfig(), client->getBuffer());

                // If nothing to send then skip
                if (request.getResponse().empty()) continue;
                // Only one send per client per poll()
                client->send(request.getResponse());

                // Cleanup after sending
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
    // Clear monitored descriptors
    _pollFds.clear();

    // Delete all server objects created with new
    for (std::map<int, Server*>::iterator it = _serversMap.begin(); it != _serversMap.end(); it++) {
        delete it->second;
    }
    _serversMap.clear();

    // Delete all client objects created with new
    for (std::map<int, Client*>::iterator it = _clientsMap.begin(); it != _clientsMap.end(); it++) {
        delete it->second;
    }
    _clientsMap.clear();

    std::cout << "ServerManager destroyed" << std::endl;
}
