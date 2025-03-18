#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class Path : public Directive
{
public:
    Path();
    void print();
    void parse(std::string &config);
    std::string getValue() const;
    bool getIsExact() const;
    Path *clone() const;

private:
    std::string _value;
    bool _isExact;
};