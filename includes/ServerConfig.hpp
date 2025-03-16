#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include "Location.hpp"
#include "Exception.hpp"

class ServerConfig
{
private:
    std::string _maxBodySize;
    std::string _host;
    std::string _port;
    std::string _serverName;
    std::map<int, std::string> _errorPages;     // status -> path
    std::map<std::string, Location> _locations; // path

public:
    ServerConfig();

    void print();

    // getters
    std::string getHost() const;
    std::string getPort() const;
    std::string getServerName() const;
    std::string getMaxBodySize() const;
    std::map<int, std::string> getErrorPages() const;
    std::map<std::string, Location> getLocations() const;

    // setters
    void setHost(std::string host);
    void setPort(std::string port);
    void setServerName(std::string serverName);
    void setMaxBodySize(std::string maxBodySize);

    void addErrorPage(int status, std::string path);
};
