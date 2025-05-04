#pragma once

#include <iostream>
#include <string>
#include <map>

#include "Directive.hpp"
#include "LocationDirective.hpp"

class Locations : public Directive {
   public:
    Locations();
    ~Locations();
    Locations(const Locations &other);

    const std::map<std::string, LocationDirective> &getValue() const;
    void parse(std::string &config);
    void print() const;
    Locations *clone() const;

   private:
    std::map<std::string, LocationDirective> _value;
};