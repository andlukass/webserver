#include "../../includes/directives/MultiDirective.hpp"

MultiDirective::MultiDirective(const std::string &name) : Directive(name) {}

void MultiDirective::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    std::vector<std::string> arguments = Utils::split(fullValue, "\n \t");
    if (!arguments.size()) throw Exception("Invalid arguments at " + this->_name + " directive");
    for (size_t i = 0; i < arguments.size(); i++) this->_value.push_back(arguments[i]);
}

void MultiDirective::print() const {
    std::cout << this->_name << ": " << Utils::concatStringVector(this->_value) << std::endl;
}

std::vector<std::string> MultiDirective::getValue() const { return this->_value; }

MultiDirective *MultiDirective::clone() const { return new MultiDirective(*this); }
