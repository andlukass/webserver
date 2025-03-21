#include "../../includes/directives/Listen.hpp"

Listen::Listen() : Directive("listen") {
    this->_ip = "0.0.0.0";
    this->_port = "80";
};

void Listen::checkPort() {
    if (this->_port.find_first_not_of("0123456789") != std::string::npos)
        throw Exception("Invalid port chars at listen directive.");
    this->_portInt = atoi(this->_port.c_str());
    if (this->_port.size() > 5 || (this->_portInt < 1 || this->_portInt > 65535))
        throw Exception("The port range is from 1 to 65535.");
}

void Listen::checkIp() {
    if (this->_ip == "localhost")
        this->_ip = "127.0.0.1";
    else if (this->_ip == "*")
        this->_ip = "0.0.0.0";

    if (this->_ip.find(".") == std::string::npos) {
        if (this->_ip.find_first_not_of("0123456789") != std::string::npos)
            throw Exception("Invalid ip chars at listen directive.");
        return;
    }
    std::vector<std::string> ip = Utils::split(this->_ip, ".");
    if (ip.size() != 4) throw Exception("Ip is invalid at listen directive.");
    for (size_t i = 0; i < ip.size(); i++) {
        if (ip[i].find_first_not_of("0123456789") != std::string::npos)
            throw Exception("Invalid ip chars at listen directive.");
        int toInt = atoi(ip[i].c_str());
        if (ip[i].size() > 3 || toInt < 0 || toInt > 255)
            throw Exception("Invalid ip value at listen directive.");
    }
}

void Listen::parseIPV6(std::string &config) {
    throw Exception("IPV6 address are not handled.");
    std::string host = "";

    if (config[0] != '[') throw Exception("Invalid IPV6 address at listen directive.");
}

void Listen::parseIPV4(std::string &config) {
    std::vector<std::string> arguments = Utils::split(config, ":");
    if (arguments.size() > 2) throw Exception("Invalid arguments at listen directive");
    if (arguments.size() == 1 && arguments[0].find(".") == std::string::npos)
        this->_port = arguments[0];
    else if (arguments.size() == 1)
        this->_ip = arguments[0];
    else {
        this->_ip = arguments[0];
        this->_port = arguments[1];
    }
    this->checkPort();
    this->checkIp();
}

void Listen::parse(std::string &config) {
    std::string fullValue = this->getFullValue(config, ';');

    Utils::trimStart(fullValue);
    if (fullValue[0] == '[')
        parseIPV6(fullValue);
    else
        parseIPV4(fullValue);
}

void Listen::print() const {
    std::cout << this->_name << ": " << this->_ip << ":" << this->_port << std::endl;
}

std::string Listen::getIp() const { return this->_ip; }

std::string Listen::getPort() const { return this->_port; }

int Listen::getPortInt() const { return this->_portInt; }

Listen *Listen::clone() const { return new Listen(*this); }
