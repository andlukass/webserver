#ifndef LISTEN_HPP
#define LISTEN_HPP

#include <iostream>
#include <string>

#include "Directive.hpp"

class Listen : public Directive {
   public:
    Listen();
    void print() const;
    int getPortInt() const;
    std::string getIp() const;
    std::string getPort() const;
    void parse(std::string &config);
    Listen *clone() const;

   private:
    std::string _ip;
    std::string _port;
    int _portInt;

    void parseIPV6(std::string &config);
    void parseIPV4(std::string &config);
    void checkIp();
    void checkPort();
};

#endif