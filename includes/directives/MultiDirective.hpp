#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class MultiDirective : public Directive
{
public:
    MultiDirective(const std::string &name);
    void print();
    void parse(std::string &config);
    std::vector<std::string> getValue() const;
    MultiDirective *clone() const;

private:
    std::vector<std::string> _value;
};