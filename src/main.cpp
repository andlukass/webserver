#include <cstdlib>
#include <iostream>

#include "../includes/Exception.hpp"
#include "../includes/Server.hpp"
#include "../includes/ServerConfig.hpp"
#include "../includes/ServerManager.hpp"
#include "../includes/cgi/CgiHandler.hpp"

// TODO: check for leaks before submitting program

enum { NONE, TEST, CGI_MODE };

int handleFlags(int argc, char *argv[]) {
    if (argc > 3) throw Exception("Invalid number of options, try -h");
    if (argc == 2) return NONE;
    if (std::string(argv[2]) == "-h") {
        std::cout << "Usage: " << argv[0] << " <config_file> [option]\n";
        std::cout << "Options: (only one per run)\n";
        std::cout << "-t: test the configuration file\n";
        std::cout << "-c: run in CGI mode\n";
        std::cout << "-h: show this help message\n";
        exit(0);
    } else if (std::string(argv[2]) == "-t") {
        return TEST;
    } else if (std::string(argv[2]) == "-c") {
        return CGI_MODE;
    } else
        throw Exception("Invalid flag, try -h");
}

int main(int argc, char *argv[]) {
    if (argc < 2) return (std::cout << "Usage: " << argv[0] << " <config_file> [option]\n", 1);
    try {
        int flag = handleFlags(argc, argv);

        ServerConfig serverConfig(argv[1]);
        if (flag == TEST) return (serverConfig.print(), 0);

        // if (flag == CGI_MODE) {
        //     CgiHandler cgi("./cgi/test.py");
        //     std::string response = cgi.execute();
        //     std::cout << "CGI output:\n" << response << std::endl;
        //     return 0;
        // }

        ServerManager allServers(serverConfig);
        allServers.run();
    } catch (std::exception &e) {
        return (std::cout << e.what() << "\n", 1);
    }
}
