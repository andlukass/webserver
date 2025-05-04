#pragma once

#include <iostream>
#include <map>
#include <string>

#include "ClientMaxBodySize.hpp"
#include "Directive.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"
#include "Locations.hpp"
#include "MultiDirective.hpp"
#include "Root.hpp"
#include "Index.hpp"

class ServerDirective : public Directive {
   public:
    ServerDirective();
    ServerDirective(const ServerDirective &other);
    ~ServerDirective();

    void parse(std::string &config);
    void print() const;
    ServerDirective *clone() const;

    Listen *getListen() const;
    ClientMaxBodySize *getClientMaxBodySize() const;
    ErrorPage *getErrorPage() const;
    Root *getRoot() const;
    size_t getLocationsCount() const;
    const LocationDirective &getLocation(std::string path) const;
    AllowMethods *getAllowMethods() const;
    Index *getIndex() const;
    MultiDirective *getServerName() const;

   private:
    DirectiveMap _value;
};