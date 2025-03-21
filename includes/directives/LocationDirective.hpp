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

class LocationDirective : public Directive {
   public:
    LocationDirective();
    LocationDirective(const LocationDirective &other);
    ~LocationDirective();

    void parse(std::string &config);
    void print() const;
    LocationDirective *clone() const;

    Autoindex *getAutoindex() const;
    AllowMethods *getAllowMethods() const;
    Root *getRoot() const;
    Path *getPath() const;
    MultiDirective *getIndex() const;

   private:
    DirectiveMap _value;
};