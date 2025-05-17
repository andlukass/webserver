#pragma once

#include <iostream>
#include <map>
#include <string>

#include "AllowMethods.hpp"
#include "Autoindex.hpp"
#include "Directive.hpp"
#include "ErrorPage.hpp"
#include "Index.hpp"
#include "Path.hpp"
#include "Root.hpp"
#include "FilePathDirective.hpp"

class LocationDirective : public Directive {
   public:
    LocationDirective();
    LocationDirective(const LocationDirective &other);
    LocationDirective &operator=(const LocationDirective &other);
    ~LocationDirective();

    void parse(std::string &config);
    void print() const;
    LocationDirective *clone() const;

    Autoindex *getAutoindex() const;
    AllowMethods *getAllowMethods() const;
    ErrorPage *getErrorPage() const;
    Root *getRoot() const;
    Path *getPath() const;
    FilePathDirective *getRedirect() const;
    FilePathDirective *getIndex() const;

   private:
    DirectiveMap _value;
};