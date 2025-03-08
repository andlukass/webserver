#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include "Location.hpp"

class ServerConfig
{
private:
    std::string _maxBodySize;
    std::string _host;
    std::string _port;
    std::string _serverName;
    std::map<int, std::string> _errorPages;
    std::map<std::string, Location> _locations; // path

public:
    ServerConfig();

    void print();

    // getters
    std::string getHost() const;
    std::string getPort() const;
    std::string getServerName() const;
    std::string getBodySize() const;
    std::map<int, std::string> getErrorPages() const;
    std::map<std::string, Location> getLocations() const;

    // setters
    void setHost(std::string host);
    void setPort(std::string port);
    void setServerName(std::string serverName);
    void setMaxBodySize(std::string maxBodySize);
};
