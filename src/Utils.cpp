#include "../includes/Utils.hpp"

std::string Utils::readFile(const std::string &filePath) {
    std::ifstream file(
        filePath.c_str(),
        std::ios::in |
            std::ios::binary);  // it is needed because if it's error it has to read again

    // read file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    file.close();  // close the file after reading
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

    // if (normalized.empty() || normalized[0] != '/') {
    //     normalized = '/' + normalized;
    // }

    return normalized;
}

bool Utils::isDirectory(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFMT) == S_IFDIR;
}

bool Utils::isFile(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFMT) == S_IFREG;
}

std::string Utils::removeLastPathLevel(const std::string &path) {
    if (path.empty()) return "/";

    std::string cleanPath = path;

    if (cleanPath.size() > 1 && cleanPath[cleanPath.size() - 1] == '/')
        cleanPath.erase(cleanPath.size() - 1);

    std::string::size_type lastSlash = cleanPath.rfind('/');

    if (lastSlash == std::string::npos || lastSlash == 0) return "/";

    return cleanPath.substr(0, lastSlash + 1);
}

bool Utils::hasProtocol(const std::string &url) {
    std::vector<std::string> parts = Utils::split(url, "://");
    if (parts.size() < 2) return false;
    size_t protoLen = parts[0].length();
    if (protoLen == 0) return false;
    if (url.substr(protoLen, 3) == "://") return true;
    return false;
}