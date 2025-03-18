#pragma once

#include <string>
#include <iostream>
#include "Directive.hpp"

class Listen : public Directive
{
public:
    Listen();
    void print();
    int getPortInt() const;
    std::string getIp() const;
    std::string getPort() const;
    void parse(std::string &config);

private:
    std::string _ip;
    std::string _port;
    int _portInt;

    void parseIPV6(std::string &config);
    void parseIPV4(std::string &config);
    void checkIp();
    void checkPort();
};