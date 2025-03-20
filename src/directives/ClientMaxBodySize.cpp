#include "../../includes/directives/ClientMaxBodySize.hpp"

ClientMaxBodySize::ClientMaxBodySize() : Directive("client_max_body_size")
{
    this->_value = "1M";
};

bool validate(std::string str)
{
    long long num;

    Utils::trimStart(str);
    size_t endNumbers = str.find_first_not_of("0123456789");
    if (endNumbers == std::string::npos)
        endNumbers = str.size();
    if (endNumbers > 10 || endNumbers == 0)
        return false;

    std::string numbers = str.substr(0, endNumbers);
    str = str.substr(endNumbers, str.size());
    if (std::string("MmGgKk").find(std::string(1, str[0])) != std::string::npos)
        str.erase(0, 1);
    Utils::trimStart(str);
    if (str[0])
        return false;

    std::istringstream ss(numbers);
    if (!(ss >> num))
        throw Exception("Invalid number passed to isValidSizeValue");
    if (num > 2147483647 || num < 0)
        return false;

    return true;
}

void ClientMaxBodySize::parse(std::string &config)
{
    std::string fullValue = this->getFullValue(config, ';');
    this->_value = fullValue;
    if (!validate(this->_value))
        throw Exception("Invalid prop: \"" + this->_value + "\" in client_max_body_size");
}

void ClientMaxBodySize::print()
{
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string ClientMaxBodySize::getValue() const
{
    return this->_value;
}

ClientMaxBodySize *ClientMaxBodySize::clone() const
{
    return new ClientMaxBodySize(*this);
}