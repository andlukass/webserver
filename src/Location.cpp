#include "../includes/Location.hpp"

Location::Location(const std::string &path, const bool &isExact)
{
    this->_path = path;
    this->_isExact = isExact;
    this->_autoindex = "off";
    this->_root = "";
}

std::string Location::getPath() const { return this->_path; }
std::string Location::getRoot() const { return this->_root; }
std::vector<std::string> Location::getIndex() const { return this->_index; }
std::string Location::getAutoindex() const { return this->_autoindex; }
bool Location::getIsExact() const { return this->_isExact; }
std::vector<std::string> Location::getAllowMethods() const { return this->_allow_methods; }

void Location::setRoot(std::string root) { this->_root = root; }
void Location::setAutoindex(std::string autoindex)
{
    if (autoindex != "on" && autoindex != "off")
        throw Exception("Invalid value: \"" + autoindex + "\" for autoindex directive");
    this->_autoindex = autoindex;
}
void Location::setIsExact(bool isExact) { this->_isExact = isExact; }
void Location::addIndex(std::string index) { this->_index.push_back(index); }
void Location::addAllowMethod(std::string method) { this->_allow_methods.push_back(method); }