#pragma once

#include "Exception.hpp"
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <vector>
#include <map>

class Utils
{

public:
    static std::string readFile(const std::string &filePath);

    static std::string trimStart(std::string &str);

    static std::string trim(std::string &str);

    static std::string concatStringVector(const std::vector<std::string> &vec);

    static std::string concatConstChars(const char *str[]);

    static bool constCharsIncludes(const char *str[], const std::string &toFind);

    static std::vector<std::string> split(const std::string &str, const std::string &delimiters);

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

    // template <typename KeyType, typename ValueType>
    // static bool mapContains(const std::map<KeyType, ValueType> &m, const KeyType &key)
    // {
    //     // Verifica se a chave existe no mapa
    //     typename std::map<KeyType, ValueType>::const_iterator it = m.find(key);
    //     return it != m.end();
    // }
};
