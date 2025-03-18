#pragma once

#include <string>
#include <iostream>
#include <map>
#include "Directive.hpp"
#include "ClientMaxBodySize.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"
#include "MultiDirective.hpp"
#include "Location.hpp"

class ServerDirective : public Directive
{
public:
    ServerDirective();
    ServerDirective(const ServerDirective &other);
    ~ServerDirective();

    std::map<std::string, Directive *> getValue() const;
    void parse(std::string &config);
    void print();
    ServerDirective *clone() const;

private:
    std::map<std::string, Directive *> _value;
};