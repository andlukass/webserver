#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class Autoindex : public Directive {
   public:
    Autoindex();
    void print() const;
    void parse(std::string &config);
    std::string getValue() const;
    Autoindex *clone() const;

   private:
    std::string _value;
};

#endif