#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/stat.h>

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Exception.hpp"

class Utils {
   public:
    static std::string readFile(const std::string &filePath);

    static std::string trimStart(std::string &str);

    static std::string trim(std::string &str);

    static std::string concatStringVector(const std::vector<std::string> &vec);

    static std::string concatConstChars(const char *str[]);

    static bool constCharsIncludes(const char *str[], const std::string &toFind);

    static std::vector<std::string> split(const std::string &str, const std::string &delimiters);

    static std::string cleanSlashes(const std::string &path);

    static bool isDirectory(const std::string &path);

    static bool isFile(const std::string &path);

    static std::string removeLastPathLevel(const std::string &path);

    static bool hasProtocol(const std::string &url);
};

#endif