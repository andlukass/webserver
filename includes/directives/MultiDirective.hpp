#ifndef MULTI_DIRECTIVE_HPP
#define MULTI_DIRECTIVE_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class MultiDirective : public Directive {
   public:
    MultiDirective(const std::string &name);
    void print() const;
    void parse(std::string &config);
    std::vector<std::string> getValue() const;
    MultiDirective *clone() const;

   private:
    std::vector<std::string> _value;
};

#endif