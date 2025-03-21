#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig(const std::string &filePath) {
    // std::cout << "ServerConfig default constructor ========================================" <<
    // std::endl;
    this->_config = Utils::readFile(filePath);
    if (this->_config.empty()) throw Exception("Error reading file: " + filePath);
    this->removeComments();
    this->parse();
}

ServerConfig::~ServerConfig() {
    // std::cout << "ServerConfig destructor ======================================== serverssize: "
    // << this->_serversConfig.size() << std::endl;
}

void ServerConfig::print() const {
    for (size_t i = 0; i < this->_serversConfig.size(); ++i) {
        std::cout << "server " << i << ": " << std::endl;
        this->_serversConfig[i].print();
    }
}

void ServerConfig::removeComments() {
    size_t pos = 0;
    while ((pos = this->_config.find("#", pos)) != std::string::npos) {
        size_t end = this->_config.find("\n", pos);
        if (end == std::string::npos) end = this->_config.size();
        this->_config.erase(pos, end - pos);
    }
}

size_t ServerConfig::getServersCount() const { return this->_serversConfig.size(); }

const ServerDirective &ServerConfig::getServer(size_t index) const {
    if (index >= this->_serversConfig.size()) throw Exception("Server index out of range");
    return this->_serversConfig[index];
}

void ServerConfig::parse() {
    std::string directive = Directive::getNextWord(this->_config);
    while (!directive.empty()) {
        if (directive != "server")
            throw Exception("Unexpected prop: \"" + directive + "\" expecting: \"server\"");
        ServerDirective server;
        server.parse(this->_config);
        this->_serversConfig.push_back(server);
        directive = Directive::getNextWord(this->_config);
    }
}