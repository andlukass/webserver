#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "./includes/ServerConfig.hpp"
#include "./includes/Exception.hpp"
#include "./includes/Parser.hpp"

void handleFlags(int argc, char *argv[])
{
    if (argc > 3)
        throw Exception("Invalid number of options, try -h");
    if (argc == 2)
        return;
    if ((std::string(argv[2]) != "-t" && std::string(argv[2]) != "-h"))
        throw Exception("Invalid flag, try -h");
    if (std::string(argv[2]) == "-h")
    {
        std::cout << "Usage: " << argv[0] << " <config_file> [option]\n";
        std::cout << "Options: (only one per run)\n";
        std::cout << "-t: test the configuration file\n";
        std::cout << "-h: show this help message\n";
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <config_file> [option]\n";
        return 1;
    }
    try
    {
        handleFlags(argc, argv);
        Parser parser(argv[1]);

        parser.parse();

        std::vector<ServerConfig> servers = parser.getServerList();
        for (size_t i = 0; i < servers.size(); ++i)
        {
            std::cout << i << ": " << std::endl;
            servers[i].print();
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << "\n";
        return 1;
    };
}
