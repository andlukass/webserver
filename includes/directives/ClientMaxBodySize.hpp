#pragma once

#include <iostream>
#include <string>

#include "Directive.hpp"

#define DEFAULT_BODY_SIZE_BYTES 1048576

class ClientMaxBodySize : public Directive {
   public:
    ClientMaxBodySize();
    size_t getValue() const;
    void parse(std::string &config);
    void print() const;
    ClientMaxBodySize *clone() const;

   private:
    std::string _value;
    size_t _byteLimit;

    size_t ClientMaxBodySize::toBytes(const std::string &input);
};