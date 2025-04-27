#include "../../includes/directives/Index.hpp"

Index::Index() : Directive("index") { this->_value = ""; }

void Index::validate() {
    std::vector<std::string> arguments = Utils::split(this->_value, "\n \t");
    if (!arguments.size() || arguments.size() > 1)
        throw Exception("Invalid arguments at " + this->_name + " directive");
}

void Index::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    this->_value = fullValue;
    this->validate();
    if (this->_value.at(0) != '/') {
        this->_value = "/" + this->_value;
    }
}

void Index::print() const {
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string Index::getValue() const { return this->_value; }

Index *Index::clone() const { return new Index(*this); }
