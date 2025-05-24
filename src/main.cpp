#include <cstdlib>
#include <iostream>

#include "../includes/Exception.hpp"
#include "../includes/Server.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/ServerManager.hpp"
#include "../includes/cgi/CgiHandler.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) return (std::cout << "Usage: " << argv[0] << " <config_file> \n", 1);
    try {
        ServerConfig serverConfig(argv[1]);
        ServerManager allServers(serverConfig);
        allServers.run();
    } catch (std::exception &e) {
        return (std::cout << e.what() << "\n", 1);
    }
}
