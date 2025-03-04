#pragma once

#include <string>
#include <fstream>
#include "ServerConfig.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

class Parser
{
private:
    std::string _rawFile;
    std::vector<ServerConfig> _serversList;

    void parseServerDirective(std::string &config);
    void parseLocationDirective(std::string &config, ServerConfig &server);
    void parseErrorPageDirective(std::string &config, ServerConfig &server);

    std::string getSimpleDirectiveValue(std::string &config);

public:
    Parser(const std::string &configPath);

    void parse();

    void addServerConfig(ServerConfig newServer);

    std::vector<ServerConfig> getServerList();
};
