#include "../../includes/directives/Directive.hpp"

std::string Directive::getNextWord(std::string &config) {
    while (config[0] && (std::isspace(config[0]))) config.erase(0, 1);
    int separatorIndex = (config).find_first_of(" \n\t{};");
    if (separatorIndex == -1) return "";
    std::string word = config;
    if (separatorIndex == 0)
        word.erase(1, config.length());
    else
        word.erase(separatorIndex, word.length());
    config.erase(0, word.length());
    // std::cout << "got>> " << word << "\n";
    return word;
}

std::string Directive::getFullValue(std::string &config, char closeChar) {
    std::string directive = "";
    std::string nextWord = getNextWord(config);

    // if (this->_name == "location")
    // {

    //     std::cout << this->_name << "============================\n entered getFullValue" <<
    //     std::endl; std::cout << config << "\n\n";
    // }

    while (nextWord != std::string(1, closeChar) && !nextWord.empty()) {
        directive += nextWord + " ";
        nextWord = getNextWord(config);
        // std::cout << "config: \"" << config << "\"" << std::endl;
        // std::cout << "nextWord: \"" << nextWord << "\"" << std::endl;
    }
    if (nextWord != std::string(1, closeChar))
        throw Exception("Unclosed directive, expecting \"" + std::string(1, closeChar) + "\". At " +
                        this->_name + " directive");
    Utils::trim(directive);
    return directive;
}
