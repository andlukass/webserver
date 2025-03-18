#pragma once

#include <string>
#include <iostream>
#include "./Exception.hpp"
#include "./Utils.hpp"
#include "./directives/ServerDirective.hpp"

class ServerConfig
{
public:
    ServerConfig(const std::string &filePath);
    void print();

private:
    std::string _config;
    std::vector<ServerDirective> _serversConfig;

    void parse();
    void removeComments();
};