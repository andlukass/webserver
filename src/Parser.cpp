#include "../includes/Parser.hpp"

const char *validServerProps[] = {"location", "listen", "server_name", "error_page", "client_max_body_size", NULL};
const char *validLocationProps[] = {"root", "index", "autoindex", "allowed_https", NULL};

Parser::Parser(const std::string &configPath)
{
    this->_rawFile = Utils::readFile(configPath);
    if (!this->_rawFile.length())
        throw Exception("Error reading configuration file");
}

int findSeparatorIndex(const std::string &str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        char ch = str[i];
        if (!(std::isalnum(ch) || ch == '_' || ch == '.' || ch == '-'))
        {
            return i;
        }
    }
    return -1;
}

std::string getNextWord(std::string &str)
{
    while (str[0] && (std::isspace(str[0])))
        str.erase(0, 1);
    int separatorIndex = findSeparatorIndex(str);
    if (separatorIndex == -1)
        return str;
    std::string word = str;
    if (separatorIndex == 0)
        word.erase(1, str.length());
    else
        word.erase(separatorIndex, word.length());
    str.erase(0, word.length());
    // std::cout << "got>> " << word << "\n";
    return word;
}

void Parser::parseLocationDirective(std::string &config, ServerConfig &server)
{
    std::string path = getNextWord(config);
    Location location(path);
    std::string initiator = getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\"");
    while (config[0])
    {
        std::string nextWord = getNextWord(config);
        if (nextWord == "}")
            return;
    }
    throw Exception("Location directive is unclosed, expecting: \"}\"");
}

std::string Parser::getSimpleDirectiveValue(std::string &config)
{
    std::string nextWord = getNextWord(config);
    std::string terminator = getNextWord(config);
    if (terminator != ";")
        throw Exception("Unexpected token: \"" + terminator + "\" expecting: \";\"");
    return nextWord;
}

void Parser::parseServerDirective(std::string &config)
{
    ServerConfig server;

    std::string nextWord = getNextWord(config);
    if (nextWord != "{")
        throw Exception("Unexpected token: \"" + std::string(1, nextWord[0]) + "\" expecting: \"{\"");

    while (config[0])
    {
        nextWord = getNextWord(config);
        if (nextWord == "listen")
            server.setListen(this->getSimpleDirectiveValue(config));
        else if (nextWord == "server_name")
            server.setServerName(this->getSimpleDirectiveValue(config));
        else if (nextWord == "client_max_body_size")
            server.setMaxBodySize(this->getSimpleDirectiveValue(config));
        else if (nextWord == "location")
            this->parseLocationDirective(config, server);
        else if (nextWord == "error_page")
            server.getBodySize();
        else if (nextWord == "}" || nextWord.empty())
            break;
        else
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" + Utils::concatConstChars(validServerProps) + "\"");
    }
    if (nextWord != "}")
        throw Exception("Server directive is unclosed, expecting: \"}\"");
    this->addServerConfig(server);
}

std::vector<ServerConfig> Parser::getServerList()
{
    return this->_serversList;
}

void Parser::parse()
{
    std::string str = this->_rawFile;

    std::string nextWord = getNextWord(str);
    if (nextWord != "server")
        throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"server\"");
    parseServerDirective(str);
}

void Parser::addServerConfig(ServerConfig newServer)
{
    this->_serversList.push_back(newServer);
}