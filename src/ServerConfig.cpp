#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &filePath)
{
    this->_config = Utils::readFile(filePath);
    if (this->_config.empty())
        throw Exception("Error reading file: " + filePath);
    this->removeComments();
    this->parse();
}

void ServerConfig::print()
{
    for (size_t i = 0; i < this->_serversConfig.size(); ++i)
    {
        std::cout << "server " << i << ": " << std::endl;
        this->_serversConfig[i].print();
    }
}

void ServerConfig::removeComments()
{
    size_t pos = 0;
    while ((pos = this->_config.find("#", pos)) != std::string::npos)
    {
        size_t end = this->_config.find("\n", pos);
        if (end == std::string::npos)
            end = this->_config.size();
        this->_config.erase(pos, end - pos);
    }
}

void ServerConfig::parse()
{
    std::string directive = Directive::getNextWord(this->_config);
    while (!directive.empty())
    {
        if (directive != "server")
            throw Exception("Unexpected prop: \"" + directive + "\" expecting: \"server\"");
        ServerDirective server;
        server.parse(this->_config);
        this->_serversConfig.push_back(server);
        directive = Directive::getNextWord(this->_config);
    }
}