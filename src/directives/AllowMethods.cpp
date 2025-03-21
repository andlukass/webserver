#include "../../includes/directives/AllowMethods.hpp"

const char *validMethods[] = {"GET",   "HEAD",  "POST",    "DELETE",  "PUT",
                              "PATCH", "TRACE", "CONNECT", "OPTIONS", NULL};

AllowMethods::AllowMethods() : Directive("allow_methods") {}

void AllowMethods::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    std::vector<std::string> arguments = Utils::split(fullValue, "\n \t");
    for (size_t i = 0; i < arguments.size(); i++) {
        if (!Utils::constCharsIncludes(validMethods, arguments[i]))
            throw Exception("Invalid method: \"" + arguments[i] + "\" in allow_methods directive");
        this->_value.push_back(arguments[i]);
    }
}

void AllowMethods::print() const {
    std::cout << this->_name << ": " << Utils::concatStringVector(this->_value) << std::endl;
}

std::vector<std::string> AllowMethods::getValue() const { return this->_value; }

AllowMethods *AllowMethods::clone() const { return new AllowMethods(*this); }
