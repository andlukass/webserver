#pragma once

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

    template <typename T>
    static bool vectorContains(std::vector<T> &vec, const T &a) {
        for (int i = 0; i < vec.size(); ++i) {
            if (a == vec[i]) return true;
        }
        return false;
    }
};
