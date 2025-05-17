#include "../../includes/directives/FilePathDirective.hpp"

FilePathDirective::FilePathDirective(const std::string& name) : Directive(name) { this->_value = ""; }

void FilePathDirective::validate() {
    std::vector<std::string> arguments = Utils::split(this->_value, "\n \t");
    if (!arguments.size() || arguments.size() > 1)
        throw Exception("Invalid arguments at " + this->_name + " directive");
    if (!Utils::hasProtocol(this->_value)) {
        this->_value = "/" + this->_value;
        this->_value = Utils::cleanSlashes(this->_value);
    } else {
        if (this->_value.size() > 50) throw Exception(this->_name + " directive path is too long");
    }
}

void FilePathDirective::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');
    this->_value = fullValue;
    this->validate();
}

void FilePathDirective::print() const {
    std::cout << this->_name << ": " << this->_value << std::endl;
}

std::string FilePathDirective::getValue() const { return this->_value; }

FilePathDirective *FilePathDirective::clone() const { return new FilePathDirective(*this); }
