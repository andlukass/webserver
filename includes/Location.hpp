#pragma once

#include <string>
#include <vector>
#include "Exception.hpp"

class Location
{
    std::string _path;
    std::string _autoindex;          // turn on/off directory listing
    std::string _root;               // path to the actual directory
    std::vector<std::string> _index; // file if is directory
    bool _isExact;                   // tells if the path must be a exact match or just start with it
    std::vector<std::string> _allow_methods;

public:
    Location(const std::string &path, const bool &isExact);

    bool getIsExact() const;
    std::string getPath() const;
    std::string getRoot() const;
    std::string getAutoindex() const;
    std::vector<std::string> getIndex() const;
    std::vector<std::string> getAllowMethods() const;

    void setIsExact(bool isExect);
    void setRoot(std::string root);
    void setAutoindex(std::string autoindex);
    void addIndex(std::string index);
    void addAllowMethod(std::string method);
};
