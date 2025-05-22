#ifndef FILEPATHDIRECTIVE_HPP
#define FILEPATHDIRECTIVE_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class FilePathDirective : public Directive {
   public:
    FilePathDirective(const std::string &name);
    void print() const;
    void parse(std::string &config);
    void validate();
    std::string getValue() const;
    FilePathDirective *clone() const;

   private:
    std::string _value;
};

#endif