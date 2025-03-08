#pragma once

#include <fstream>
#include <string>
#include <cctype>
#include <sstream>

class Utils
{

public:
    static std::string readFile(const std::string &filePath);

    static std::string trimStart(std::string &str);

    static std::string concatStringVector(const std::vector<std::string> &vec);

    static std::string concatConstChars(const char *str[]);

    static bool isValidIPv6(const std::string &address);

    template <typename T>
    static bool vectorContains(std::vector<T> &vec, const T &a)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (a == vec[i])
                return true;
        }
        return false;
    }
};
