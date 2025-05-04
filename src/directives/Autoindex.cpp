#include "../../includes/directives/Autoindex.hpp"

Autoindex::Autoindex() : Directive("autoindex") { this->_value = "off"; }

void Autoindex::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    if (fullValue != "on" && fullValue != "off")
        throw Exception("Invalid value: \"" + fullValue + "\" at " + this->_name + " directive");
    this->_value = fullValue;
}

void Autoindex::print() const {
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string Autoindex::getValue() const { return this->_value; }

Autoindex *Autoindex::clone() const { return new Autoindex(*this); }
