#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include "../Exception.hpp"
#include "../Utils.hpp"

class Directive
{
public:
    Directive(const std::string &name) : _name(name) {}

    virtual void parse(std::string &config) = 0;
    virtual void print() = 0;
    static std::string getNextWord(std::string &config);

protected:
    const std::string _name;
    std::string getFullValue(std::string &config, char closeChar);
};