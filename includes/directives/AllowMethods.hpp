#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class AllowMethods : public Directive
{
public:
    AllowMethods();
    void print();
    void parse(std::string &config);
    std::vector<std::string> getValue() const;

private:
    std::vector<std::string> _value;
};