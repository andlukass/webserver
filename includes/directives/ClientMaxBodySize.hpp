#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class ClientMaxBodySize : public Directive
{
public:
    ClientMaxBodySize();
    std::string getValue() const;
    void parse(std::string &config);
    void print();

private:
    std::string _value;
};