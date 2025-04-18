#pragma once

#include <iostream>
#include <string>

#include "Directive.hpp"

class ErrorPage : public Directive {
   public:
    ErrorPage();
    void print() const;
    void parse(std::string &config);
    std::map<int, std::string> getValue() const;
    ErrorPage *clone() const;

   private:
    std::map<int, std::string> _value;
};