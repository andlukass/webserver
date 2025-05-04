#include "../../includes/directives/ErrorPage.hpp"

ErrorPage::ErrorPage() : Directive("error_page") {};

void ErrorPage::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    std::vector<std::string> arguments = Utils::split(fullValue, "\n \t");
    if (arguments.size() <= 1) throw Exception("Invalid arguments at error_page directive");
    std::string path = arguments[arguments.size() - 1];
    for (size_t i = 0; i < arguments.size(); i++) {
        if (i == (arguments.size() - 1)) break;
        if (arguments[i].find_first_not_of("0123456789") != std::string::npos)
            throw Exception("Invalid status at error_page directive, must be only numeric");
        int status = atoi(arguments[i].c_str());
        if (status < 300 || status > 599 || arguments[i].size() > 3)
            throw Exception(
                "Invalid status at error_page directive, must be more than 300, and less than 599");
        this->_value[status] = path;
    }
}

void ErrorPage::print() const {
    std::cout << this->_name << ": \n";
    for (std::map<int, std::string>::const_iterator it = this->_value.begin();
         it != this->_value.end(); ++it) {
        std::cout << "\tstatus - " << it->first << " | page - " << it->second << std::endl;
    }
}

const std::map<int, std::string>& ErrorPage::getValue() const { return this->_value; }

ErrorPage *ErrorPage::clone() const { return new ErrorPage(*this); }
