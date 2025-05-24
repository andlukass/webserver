#include "../../includes/directives/Locations.hpp"

Locations::Locations() : Directive("locations") {
    // std::cout << "Location default constructor ===============================index: " <<
    // std::endl;
}

Locations::~Locations() {
    // std::cout << "Location destructor ===============================index: " << std::endl;
}

Locations::Locations(const Locations &other) : Directive(other._name) {
    for (std::map<std::string, LocationDirective>::const_iterator it = other._value.begin(); it != other._value.end(); it++) {
        LocationDirective* cloned = it->second.clone();
        this->_value[it->first] = *cloned;
        delete cloned;
    }
}


void Locations::parse(std::string &config) {
    LocationDirective location;
    location.parse(config);
    this->_value[location.getPath()->getValue()] = location;
}

void Locations::print() const {
    for (std::map<std::string, LocationDirective>::const_iterator it = this->_value.begin(); it != this->_value.end(); it++) {
        it->second.print();
    }
}

const std::map<std::string, LocationDirective> &Locations::getValue() const { return this->_value; }

Locations *Locations::clone() const { return new Locations(*this); }
