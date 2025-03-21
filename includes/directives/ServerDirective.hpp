#pragma once

#include <iostream>
#include <map>
#include <string>

#include "AllowMethods.hpp"
#include "Autoindex.hpp"
#include "ClientMaxBodySize.hpp"
#include "Directive.hpp"
#include "ErrorPage.hpp"
#include "Listen.hpp"
#include "Location.hpp"
#include "MultiDirective.hpp"
#include "Path.hpp"
#include "Root.hpp"

class ServerDirective : public Directive {
   public:
    ServerDirective();
    ServerDirective(const ServerDirective &other);
    ~ServerDirective();

    void parse(std::string &config);
    void print() const;
    ServerDirective *clone() const;

    Listen *getListen() const;
    Autoindex *getAutoindex() const;
    AllowMethods *getAllowMethods() const;
    ClientMaxBodySize *getClientMaxBodySize() const;
    ErrorPage *getErrorPage() const;
    Root *getRoot() const;
    Path *getPath() const;
    size_t getLocationsCount() const;
    const DirectiveMap &getLocation(size_t index) const;
    MultiDirective *getIndex() const;
    MultiDirective *getServerName() const;

   private:
    DirectiveMap _value;
};