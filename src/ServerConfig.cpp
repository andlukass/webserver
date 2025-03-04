
#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
    this->_listen = "8080";
    this->_serverName = "localhost";
    this->_locations.insert(std::make_pair("/", Location("/")));
    this->_errorPages.insert(std::pair<int, std::string>(404, "404.html"));
    this->_maxBodySize = "2m";
}

std::string ServerConfig::getListen() const
{
    return this->_listen;
}

std::string ServerConfig::getServerName() const
{
    return this->_serverName;
}

std::string ServerConfig::getBodySize() const
{
    return this->_maxBodySize;
}

std::map<int, std::string> ServerConfig::getErrorPages() const
{
    return this->_errorPages;
}

std::map<std::string, Location> ServerConfig::getLocations() const
{
    return this->_locations;
}

void ServerConfig::print()
{
    std::cout << "Listen: " << this->_listen << "\n";
    std::cout << "ServerName: " << this->_serverName << "\n";
    std::cout << "MaxBodySize: " << this->_maxBodySize << "\n";
}

void ServerConfig::setListen(std::string listen)
{
    this->_listen = listen;
}
void ServerConfig::setServerName(std::string serverName)
{
    this->_serverName = serverName;
}
void ServerConfig::setMaxBodySize(std::string maxBodySize)
{
    this->_maxBodySize = maxBodySize;
}
