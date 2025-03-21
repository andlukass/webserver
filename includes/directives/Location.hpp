#pragma once

#include <iostream>
#include <map>
#include <string>

#include "AllowMethods.hpp"
#include "Autoindex.hpp"
#include "Directive.hpp"
#include "MultiDirective.hpp"
#include "Path.hpp"
#include "Root.hpp"

class Location : public Directive {
   public:
    Location();
    ~Location();
    Location(const Location &other);

    const std::vector<DirectiveMap> &getValue() const;
    void parse(std::string &config);
    void print() const;
    void init();
    Location *clone() const;

   private:
    std::vector<DirectiveMap> _value;
};