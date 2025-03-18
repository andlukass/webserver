#include <iostream>
#include <cstdlib>
#include "../includes/Exception.hpp"
#include "../includes/ServerConfig.hpp"

enum
{
    NONE,
    TEST
};

int handleFlags(int argc, char *argv[])
{
    if (argc > 3)
        throw Exception("Invalid number of options, try -h");
    if (argc == 2)
        return NONE;
    if (std::string(argv[2]) == "-h")
    {
        std::cout << "Usage: " << argv[0] << " <config_file> [option]\n";
        std::cout << "Options: (only one per run)\n";
        std::cout << "-t: test the configuration file\n";
        std::cout << "-h: show this help message\n";
        exit(0);
    }
    else if (std::string(argv[2]) == "-t")
        return TEST;
    else
        throw Exception("Invalid flag, try -h");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return (std::cout << "Usage: " << argv[0] << " <config_file> [option]\n", 1);
    try
    {
        int flag = handleFlags(argc, argv);

        ServerConfig serverConfig(argv[1]);
        if (flag == TEST)
            return (serverConfig.print(), 0);
        std::cout << "Server started.....( not really :') )\n";
        while (1)
            ;
        return (0);
    }
    catch (std::exception &e)
    {
        return (std::cout << e.what() << "\n", 1);
    };
}
