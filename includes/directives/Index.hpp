#ifndef INDEX_HPP
#define INDEX_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class Index : public Directive {
   public:
    Index();
    void print() const;
    void parse(std::string &config);
    void validate();
    std::string getValue() const;
    Index *clone() const;

   private:
    std::string _value;
};

#endif