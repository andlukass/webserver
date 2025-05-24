#include "../../includes/directives/Path.hpp"

Path::Path() : Directive("path") {
    this->_value = "";
}

void Path::parse(std::string &config) {
    this->_value = getNextWord(config);
    std::vector<std::string> slashs = Utils::split(this->_value, "/");
    if (slashs.size() > 1) throw Exception("Path directive must be a single path");
    this->_value = Utils::cleanSlashes(this->_value);
	if (this->_value[this->_value.size() - 1] != '/')
        this->_value += '/';
    if (this->_value.size() > 50) throw Exception("Path directive path is too long");
}

void Path::print() const {
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string Path::getValue() const { return this->_value; }

Path *Path::clone() const { return new Path(*this); }
