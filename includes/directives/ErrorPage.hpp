#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class ErrorPage : public Directive
{
public:
    ErrorPage();
    void print();
    void parse(std::string &config);
    std::map<int, std::string> getValue() const;

private:
    std::map<int, std::string> _value;
};