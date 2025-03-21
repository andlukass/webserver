#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Directive.hpp"
#include "LocationDirective.hpp"

class Locations : public Directive {
   public:
    Locations();
    ~Locations();
    Locations(const Locations &other);

    const std::vector<LocationDirective> &getValue() const;
    void parse(std::string &config);
    void print() const;
    Locations *clone() const;

   private:
    std::vector<LocationDirective> _value;
};