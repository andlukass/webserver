#ifndef PATH_HPP
#define PATH_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class Path : public Directive {
   public:
    Path();
    void print() const;
    void parse(std::string &config);
    std::string getValue() const;
    Path *clone() const;

   private:
    std::string _value;
};

#endif