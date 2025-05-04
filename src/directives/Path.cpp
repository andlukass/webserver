#include "../../includes/directives/Path.hpp"

Path::Path() : Directive("path") {
    this->_value = "";
    this->_isExact = false;
}

void Path::parse(std::string &config) {
    this->_value = getNextWord(config);
    if (this->_value == "=") {
        this->_isExact = true;
        this->_value = getNextWord(config);
    }
    this->_value = Utils::cleanSlashes(this->_value);
}

void Path::print() const {
    std::string exact = this->_isExact ? "= " : "";
    std::cout << this->_name << ": " << exact << this->_value << std::endl;
}

std::string Path::getValue() const { return this->_value; }

bool Path::getIsExact() const { return this->_isExact; }

Path *Path::clone() const { return new Path(*this); }
