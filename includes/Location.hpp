#pragma once

#include <string>
#include <vector>

class Location
{
    std::string path;
    std::string root;      // path to the actual directory
    std::string index;     // file if is directory
    std::string autoindex; // turn on/off directory listing
    std::vector<std::string> denyHttps;

public:
    Location(const std::string &path)
    {
    }

    std::string getRoot() const { return this->root; }
    std::string getIndex() const { return this->index; }
    std::string getAutoindex() const { return this->autoindex; }
    std::vector<std::string> getDenyHttps() const { return this->denyHttps; }
};
