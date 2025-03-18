#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class Autoindex : public Directive
{
public:
    Autoindex();
    void print();
    void parse(std::string &config);
    bool getValue() const;

private:
    bool _value;
};