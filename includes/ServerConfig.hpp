// TODO: (Yulia) make sure that this allowed. As far as I know it's not C++98 standard :(
#pragma once

#include <iostream>
#include <string>

#include "./Exception.hpp"
#include "./Utils.hpp"
#include "./directives/ServerDirective.hpp"

class ServerConfig {
   public:
    ServerConfig(const std::string &filePath);
    ~ServerConfig();
    void print() const;
    size_t getServersCount() const;
    const ServerDirective &getServer(size_t index) const;

   private:
    std::string _config;
    std::vector<ServerDirective> _serversConfig;

    void parse();
    void removeComments();
};