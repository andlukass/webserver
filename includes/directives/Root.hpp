#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class Root : public Directive
{
public:
    Root();
    void print();
    void parse(std::string &config);
    std::string getValue() const;

private:
    std::string _value;
};