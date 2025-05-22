#ifndef ROOT_HPP
#define ROOT_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class Root : public Directive {
   public:
    Root();
    void print() const;
    void parse(std::string &config);
    std::string getValue() const;
    Root *clone() const;

   private:
    std::string _value;
};

#endif