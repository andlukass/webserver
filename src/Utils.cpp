#include "../includes/Utils.hpp"

std::string Utils::readFile(const std::string &filePath) {
 std::ifstream file(
        filePath.c_str(),
        std::ios::in |
            std::ios::binary);  // it is needed because if it's error it has to read again

    // read file content into a string, maybe I should use this at the http header file
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    file.close();          // close the file after reading
    return (fileContent);
}

std::string Utils::trimStart(std::string &str) {
    while (str[0] && (std::isspace(str[0]))) str.erase(0, 1);
    return str;
}

std::string Utils::trim(std::string &str) {
    trimStart(str);
    size_t end = str.size();
    while (end > 0 && std::isspace(str[end - 1])) --end;
    str = str.substr(0, end);
    return str;
}

std::string Utils::concatStringVector(const std::vector<std::string> &vec) {
    std::string result;

    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i != vec.size() - 1) {
            result += ", ";
        }
    }

    return result;
}

std::string Utils::concatConstChars(const char *str[]) {
    std::string result;

    for (size_t i = 0; str[i]; i++) {
        result += std::string(str[i]);
        if (str[i + 1]) {
            result += ", ";
        }
    }

    return result;
}

bool Utils::constCharsIncludes(const char *str[], const std::string &toFind) {
    for (size_t i = 0; str[i]; i++) {
        if (std::string(str[i]) == toFind) return true;
    }
    return false;
}

std::vector<std::string> Utils::split(const std::string &str, const std::string &delimiters) {
    std::vector<std::string> result;
    size_t start = 0, end;

    while ((end = str.find_first_of(delimiters, start)) != std::string::npos) {
        if (end > start) result.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    if (start < str.size()) result.push_back(str.substr(start));

    return result;
}

std::string Utils::cleanSlashes(const std::string &path) {
    std::string normalized;
    bool lastWasSlash = false;
    
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '/') {
            if (!lastWasSlash) {
                normalized += '/';
                lastWasSlash = true;
            }
        } else {
            normalized += path[i];
            lastWasSlash = false;
        }
    }
    
    if (normalized.empty() || normalized[0] != '/') {
        normalized = '/' + normalized;
    }
    
    return normalized;
}

bool Utils::isDirectory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        // Erro ao acessar o path (por exemplo, não existe)
        return false;
    }
    return (info.st_mode & S_IFMT) == S_IFDIR;
}