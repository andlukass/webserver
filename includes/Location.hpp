#pragma once

#include <string>
#include <vector>

class Location
{
    std::string path;
    std::string root;  // path to the actual directory
    std::string index; // file if is directory
    std::string autoindex;
    std::vector<std::string> allowedHttps;
    std::vector<std::string> validProps;

public:
    Location(const std::string &path)
    {
    }

    std::string getRoot() const { return this->root; }
    std::string getIndex() const { return this->index; }
    std::string getAutoindex() const { return this->autoindex; }
    std::vector<std::string> getAllowedHttps() const { return this->allowedHttps; }
    std::vector<std::string> getValidDirectives() const { return this->validProps; }
};
