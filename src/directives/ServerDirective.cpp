#include "../../includes/directives/ServerDirective.hpp"

const char *validServerProps[] = {"listen",   "server_name", "client_max_body_size",
                                  "index",    "root",        "error_page",
                                  "location", NULL};

ServerDirective::ServerDirective() : Directive("server") {
    // std::cout << "ServerDirective default constructor ========================================"
    // << std::endl;
    this->_value["listen"] = new Listen();
    this->_value["server_name"] = new MultiDirective("server_name");
    this->_value["client_max_body_size"] = new ClientMaxBodySize();
    this->_value["index"] = new MultiDirective("index");
    this->_value["root"] = new Root();
    this->_value["error_page"] = new ErrorPage();
    this->_value["location"] = new Location();
}

ServerDirective::ServerDirective(const ServerDirective &other) : Directive(other._name) {
    // std::cout << "ServerDirective copy constructor ========================================" <<
    // std::endl;
    for (size_t i = 0; validServerProps[i]; i++)
        this->_value[validServerProps[i]] = other._value.at(validServerProps[i])->clone();
}

ServerDirective::~ServerDirective() {
    // std::cout << "ServerDirective destructor ========================================" <<
    // std::endl;

    for (size_t i = 0; validServerProps[i]; i++) {
        // std::cout << "deleting " << validServerProps[i] << std::endl;
        delete this->_value[validServerProps[i]];
    }
}

void ServerDirective::parse(std::string &config) {
    std::string initiator = this->getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\". At " +
                        this->_name + " directive");

    std::string nextWord = this->getNextWord(config);
    while (!nextWord.empty() && nextWord != "}") {
        if (!this->_value[nextWord])
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" +
                            Utils::concatConstChars(validServerProps) + "\"");
        this->_value[nextWord]->parse(config);
        nextWord = this->getNextWord(config);
    }
    if (nextWord != "}") throw Exception("Server directive is unclosed, expecting: \"}\"");
}

void ServerDirective::print() const {
    for (size_t i = 0; validServerProps[i]; i++) {
        this->_value.at(validServerProps[i])->print();
    }
}

ServerDirective *ServerDirective::clone() const { return new ServerDirective(*this); }

Listen *ServerDirective::getListen() const {
    return dynamic_cast<Listen *>(this->_value.at("listen"));
}

Autoindex *ServerDirective::getAutoindex() const {
    return dynamic_cast<Autoindex *>(this->_value.at("autoindex"));
}

AllowMethods *ServerDirective::getAllowMethods() const {
    return dynamic_cast<AllowMethods *>(this->_value.at("allow_methods"));
}

ErrorPage *ServerDirective::getErrorPage() const {
    return dynamic_cast<ErrorPage *>(this->_value.at("error_page"));
}

ClientMaxBodySize *ServerDirective::getClientMaxBodySize() const {
    return dynamic_cast<ClientMaxBodySize *>(this->_value.at("client_max_body_size"));
}

Root *ServerDirective::getRoot() const { return dynamic_cast<Root *>(this->_value.at("root")); }

Path *ServerDirective::getPath() const { return dynamic_cast<Path *>(this->_value.at("path")); }

MultiDirective *ServerDirective::getIndex() const {
    return dynamic_cast<MultiDirective *>(this->_value.at("index"));
}

MultiDirective *ServerDirective::getServerName() const {
    return dynamic_cast<MultiDirective *>(this->_value.at("server_name"));
}

size_t ServerDirective::getLocationsCount() const {
    return dynamic_cast<Location *>(this->_value.at("location"))->getValue().size();
}

const DirectiveMap &ServerDirective::getLocation(size_t index) const {
    if (index >= dynamic_cast<Location *>(this->_value.at("location"))->getValue().size())
        throw Exception("Location index out of range");
    return dynamic_cast<Location *>(this->_value.at("location"))->getValue()[index];
}