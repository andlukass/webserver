#include "../../includes/directives/Autoindex.hpp"

Autoindex::Autoindex() : Directive("autoindex")
{
    this->_value = false;
}

void Autoindex::parse(std::string &config)
{
    std::string fullValue = this->getFullValue(config, ';');
    if (fullValue != "on" && fullValue != "off")
        throw Exception("Invalid value: \"" + fullValue + "\" at " + this->_name + " directive");
    this->_value = (fullValue == "on");
}

void Autoindex::print()
{
    std::string strValue = this->_value ? "on" : "off";
    std::cout << this->_name << ": " << strValue << std::endl;
}

bool Autoindex::getValue() const
{
    return this->_value;
}