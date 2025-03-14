#include "../includes/Utils.hpp"

std::string Utils::readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file)
        return "";

    std::string linha;
    std::string finalString;
    while (std::getline(file, linha))
    {
        finalString += "\n";
        finalString += linha;
    }

    file.close();
    return finalString;
}

std::string Utils::trimStart(std::string &str)
{
    while (str[0] && (std::isspace(str[0])))
        str.erase(0, 1);
    return str;
}

std::string Utils::concatStringVector(const std::vector<std::string> &vec)
{
    std::string result;

    for (size_t i = 0; i < vec.size(); ++i)
    {
        result += vec[i];
        if (i != vec.size() - 1)
        {
            result += ", ";
        }
    }

    return result;
}

std::string Utils::concatConstChars(const char *str[])
{
    std::string result;

    for (size_t i = 0; str[i]; i++)
    {
        result += std::string(str[i]);
        if (str[i + 1])
        {
            result += ", ";
        }
    }

    return result;
}
