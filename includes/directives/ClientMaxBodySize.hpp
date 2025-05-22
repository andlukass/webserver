#ifndef CLIENT_MAX_BODY_SIZE_HPP
#define CLIENT_MAX_BODY_SIZE_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

#define DEFAULT_BODY_SIZE_BYTES 1048576

class ClientMaxBodySize : public Directive {
   public:
    ClientMaxBodySize();
    std::string getValue() const;
    size_t getValueBytes() const;
    void parse(std::string &config);
    void print() const;
    ClientMaxBodySize *clone() const;

   private:
    std::string _value;
    size_t _byteLimit;

    size_t toBytes(const std::string &input);
};

#endif