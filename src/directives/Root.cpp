#include "../../includes/directives/Root.hpp"

Root::Root() : Directive("root") {
    this->_value = "";
}

void Root::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    std::vector<std::string> arguments = Utils::split(fullValue, "\n \t");
    if (arguments.size() != 1) throw Exception("Invalid arguments at root directive");
    if (arguments[0].size() > 200) throw Exception("Root directive path is too long");
    this->_value = Utils::cleanSlashes("/" + arguments[0] + "/");
}

void Root::print() const { std::cout << this->_name << ": " << this->_value << std::endl; }

std::string Root::getValue() const { return this->_value; }

Root *Root::clone() const { return new Root(*this); }
