#ifndef SERVER_DIRECTIVE_HPP
#define SERVER_DIRECTIVE_HPP

#include <iostream>
#include <map>
#include <string>

#include "Autoindex.hpp"
#include "ClientMaxBodySize.hpp"
#include "Directive.hpp"
#include "ErrorPage.hpp"
#include "FilePathDirective.hpp"
#include "Index.hpp"
#include "Listen.hpp"
#include "Locations.hpp"
#include "MultiDirective.hpp"
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
    ClientMaxBodySize *getClientMaxBodySize() const;
    size_t getClientMaxBodySizeBytes() const;
    ErrorPage *getErrorPage() const;
    Root *getRoot() const;
    size_t getLocationsCount() const;
    const LocationDirective &getLocation(std::string path) const;
    AllowMethods *getAllowMethods() const;
    FilePathDirective *getIndex() const;
    MultiDirective *getServerName() const;
    Autoindex *getAutoindex() const;
    FilePathDirective *getRedirect() const;

   private:
    DirectiveMap _value;
};

#endif