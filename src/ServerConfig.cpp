
#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
    this->_port = "80";
    this->_host = "0.0.0.0";
    this->_serverName = "localhost";
    this->_locations.insert(std::make_pair("/", Location("/")));
    this->_errorPages.insert(std::pair<int, std::string>(404, "404.html"));
    this->_maxBodySize = "2m";
}

std::string ServerConfig::getPort() const
{
    return this->_port;
}

std::string ServerConfig::getHost() const
{
    return this->_host;
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
    std::cout << "Host: " << this->_host << "\n";
    std::cout << "Port: " << this->_port << "\n";
    std::cout << "ServerName: " << this->_serverName << "\n";
    std::cout << "MaxBodySize: " << this->_maxBodySize << "\n";
}

void ServerConfig::setPort(std::string port)
{
    this->_port = port;
}

void ServerConfig::setHost(std::string host)
{
    this->_host = host;
}

void ServerConfig::setServerName(std::string serverName)
{
    this->_serverName = serverName;
}
void ServerConfig::setMaxBodySize(std::string maxBodySize)
{
    this->_maxBodySize = maxBodySize;
}
