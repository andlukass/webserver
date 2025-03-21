#include "../../includes/directives/Locations.hpp"

Locations::Locations() : Directive("locations") {
    // std::cout << "Location default constructor ===============================index: " <<
    // std::endl;
}

Locations::~Locations() {
    // std::cout << "Location destructor ===============================index: " << std::endl;
}

Locations::Locations(const Locations &other) : Directive(other._name) {
    // std::cout << "Location copy constructor ===============================index: " << std::endl;
    for (size_t i = 0; i < other._value.size(); i++) {
        this->_value.push_back(other._value[i]);
    }
}

void Locations::parse(std::string &config) {
    LocationDirective location;
    location.parse(config);
    this->_value.push_back(location);
}

void Locations::print() const {
    for (int i = 0; i < this->_value.size(); i++) {
        this->_value[i].print();
    }
}

const std::vector<LocationDirective> &Locations::getValue() const { return this->_value; }

Locations *Locations::clone() const { return new Locations(*this); }
