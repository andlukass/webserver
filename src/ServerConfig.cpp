
#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
    this->_port = "80";
    this->_host = "0.0.0.0";
    this->_serverName = "localhost";
    this->_maxBodySize = "1M";
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

std::string ServerConfig::getMaxBodySize() const
{
    return this->_maxBodySize;
}

std::map<int, std::string> ServerConfig::getErrorPages() const
{
    return this->_errorPages;
}

std::vector<Location> ServerConfig::getLocations() const
{
    return this->_locations;
}

void ServerConfig::print()
{
    std::cout << "Host: " << this->_host << "\n";
    std::cout << "Port: " << this->_port << "\n";
    std::cout << "ServerName: " << this->_serverName << "\n";
    std::cout << "MaxBodySize: " << this->_maxBodySize << "\n";
    std::cout << "ErrorPages: " << "\n";
    for (std::map<int, std::string>::iterator it = this->_errorPages.begin(); it != this->_errorPages.end(); ++it)
    {
        std::cout << "\tStatus - " << it->first << " | Page - " << it->second << std::endl;
    }

    std::cout << "Locations: " << "\n";
    for (size_t i = 0; i < this->_locations.size(); i++)
    {
        std::cout << "\tPath -> " << this->_locations[i].getPath() << "\n"
                  << "\tIsExact: " << (this->_locations[i].getIsExact() ? "Yes" : "No") << "\n"
                  << "\tRoot: " << this->_locations[i].getRoot() << "\n"
                  << "\tIndex: " << Utils::concatStringVector(this->_locations[i].getIndex()) << "\n"
                  << "\tAutoindex: " << this->_locations[i].getAutoindex() << "\n"
                  << "\tAllowMethods: " << Utils::concatStringVector(this->_locations[i].getAllowMethods()) << "\n";
    }
    std::cout << "====================================================" << std::endl;
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

void ServerConfig::addErrorPage(int status, std::string path)
{
    if (status < 300 || status > 599)
        throw Exception("Invalid status");
    this->_errorPages[status] = path;
}

void ServerConfig::addLocation(Location location)
{
    this->_locations.push_back(location);
}
