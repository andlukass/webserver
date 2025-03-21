#pragma once

#include <iostream>
#include <string>

#include "Directive.hpp"

class ClientMaxBodySize : public Directive {
   public:
    ClientMaxBodySize();
    std::string getValue() const;
    void parse(std::string &config);
    void print() const;
    ClientMaxBodySize *clone() const;
    ClientMaxBodySize *cast() const;

   private:
    std::string _value;
};