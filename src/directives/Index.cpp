#include "../../includes/directives/Index.hpp"

Index::Index() : Directive("index") { this->_value = ""; }

void Index::validate() {
    std::cout << "TESTE DO INDEX" << this->_value << std::endl;
    std::vector<std::string> arguments = Utils::split(this->_value, "\n \t");
    if (!arguments.size() || arguments.size() > 1)
        throw Exception("Invalid arguments at " + this->_name + " directive");
}

void Index::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    this->_value = fullValue;
    this->validate();
    //if (this->_value.front() != '/')
	//CHANGED
	if (this->_value[0] != '/') {
        this->_value = "/" + this->_value;
    }
    if (this->_value.size() > 50) throw Exception("Index directive path is too long");
    this->_value = Utils::cleanSlashes(this->_value);
}

void Index::print() const {
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string Index::getValue() const { return this->_value; }

Index *Index::clone() const { return new Index(*this); }
