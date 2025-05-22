#ifndef ALLOWED_METHODS_HPP
#define ALLOWED_METHODS_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class AllowMethods : public Directive {
   public:
    AllowMethods();
    void parse(std::string &config);
    void print() const;
    std::vector<std::string> getValue() const;
    AllowMethods *clone() const;
    AllowMethods *cast() const;

   private:
    std::vector<std::string> _value;
};

#endif