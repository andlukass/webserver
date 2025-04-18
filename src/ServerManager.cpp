#include "../includes/ServerManager.hpp"

#include <poll.h>

/*
TODO: Future Configuration Validations (maybe parser already validates some or all)

1. Unique IP:Port Check
Ensure no two servers are configured with the same IP:Port.

2. Required Configuration Fields
Verify required fields like server_name, root, and listen are present and valid.

3. client_max_body_size Validation
Ensure client_max_body_size is positive and reasonable.

4. Check File Paths
Validate the existence of directories specified by root and other path-based fields.

5. Method Restrictions (allow_methods)
Ensure allow_methods contains valid HTTP methods like GET, POST, etc.

6. Error Pages
Ensure paths for custom error pages (e.g., error_page 404) are valid.
*/

ServerManager::ServerManager(const ServerConfig& config) {
    for (size_t i = 0; i < config.getServersCount(); i++) {
        const ServerDirective& directive = config.getServer(i);
        // if this causes problems, we can switch to std::vector<Server*>
        try {
            _servers.push_back(new Server(directive));
        } catch (std::exception& e) {
            throw Exception("Failed to initialize server " + config.getServer(i).getListen()->getIp() + ":" +
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
