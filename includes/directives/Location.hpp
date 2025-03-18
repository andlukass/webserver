#pragma once

#include <string>
#include <iostream>
#include <map>
#include "Directive.hpp"
#include "AllowMethods.hpp"
#include "Autoindex.hpp"
#include "Root.hpp"
#include "Path.hpp"
#include "MultiDirective.hpp"

typedef std::map<std::string, Directive *> LocationValue;

class Location : public Directive
{
public:
    Location();
    ~Location();

    std::vector<LocationValue> getValue() const;
    void parse(std::string &config);
    void print();
    void init();

private:
    int _lastIndex;
    std::vector<LocationValue> _value;
};