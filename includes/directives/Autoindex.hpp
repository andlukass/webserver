#pragma once

#include <iostream>
#include <string>

#include "Directive.hpp"

class Autoindex : public Directive {
   public:
    Autoindex();
    void print() const;
    void parse(std::string &config);
    bool getValue() const;
    Autoindex *clone() const;
    Autoindex *cast() const;

   private:
    bool _value;
};