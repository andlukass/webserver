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

bool isValidSizeValue(std::string str)
{
    long long num;

    Utils::trimStart(str);
    size_t endNumbers = str.find_first_not_of("0123456789");
    if (endNumbers == std::string::npos)
        endNumbers = str.size();
    if (endNumbers > 10 || endNumbers == 0)
        return false;

    std::string numbers = str.substr(0, endNumbers);
    str = str.substr(endNumbers, str.size());
    if (std::string("MmGgKk").find(std::string(1, str[0])) != std::string::npos)
        str.erase(0, 1);
    Utils::trimStart(str);
    if (str[0])
        return false;

    std::istringstream ss(numbers);
    if (!(ss >> num))
        throw Exception("Invalid number passed to isValidSizeValue");
    if (num > 2147483647 || num < 0)
        return false;

    return true;
}

void parseLocationDirective(std::string &config, ServerConfig &server)
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

std::string getIpOrPort(std::string &config, const std::string &alloweds)
{
    size_t closeIndex = config.find_first_not_of(alloweds);
    std::string str = config.substr(0, closeIndex);
    config.erase(0, closeIndex);
    return str;
}

void checkPort(const std::string &port, const std::string &version)
{
    if (port.empty())
        throw Exception("Listen directive is invalid. (" + version + ")");

    int toInt = std::atoi(port.c_str());
    if (port.size() > 5 || (toInt < 1 || toInt > 65535))
        throw Exception("The port range is from 1 to 65535. (" + version + ")");
}

void parseIPV6(std::string &config, ServerConfig &server)
{
    throw Exception("IPV6 address are not handled.");
    std::string host = "";

    if (config[0] != '[')
        throw Exception("Invalid IPV6 address at listen directive.");
    config.erase(0, 1);
    host = getIpOrPort(config, "ABCDEFabcdef1234567890:.");
    std::cout << "host>>>: " << host << "\n";
    server.setHost(host);
    if (config[0] != ']')
        throw Exception("Invalid IPV6 address at listen directive...");
    config.erase(0, 1);
    if (std::isspace(config[0]) || config[0] == ';')
    {
        Utils::trimStart(config);
        if (config[0] != ';')
            throw Exception("Unexpected token: \"" + std::string(1, config[0]) + "\" expecting: \";\" (IPV6)");
    }
    else if (config[0] == ':')
    {
        config.erase(0, 1);
        server.setPort(getIpOrPort(config, "0123456789"));
        checkPort(server.getPort(), "IPV6");
    }
    else
        throw Exception("Invalid address/port at listen directive. (IPV6)");
}

void parseIPV4(std::string &config, ServerConfig &server)
{
    std::string temp = "";

    temp = getIpOrPort(config, "0123456789.");
    if (temp[temp.size() - 1] == '.')
        throw Exception("Listen directive is invalid... (IPV4)");
    if (std::isspace(config[0]) || config[0] == ';')
    {
        Utils::trimStart(config);
        if (config[0] != ';')
            throw Exception("Listen directive has invalid white space (IPV4)");
        if (temp.find(".") != std::string::npos)
            server.setHost(temp);
        else
            server.setPort(temp);
    }
    else if (config[0] == ':')
    {
        config.erase(0, 1);
        server.setHost(temp);
        server.setPort(getIpOrPort(config, "0123456789"));
    }
    checkPort(server.getPort(), "IPV4");
}

void parseListenDirective(std::string &config, ServerConfig &server)
{
    std::string nextWord = "";

    Utils::trimStart(config);
    if (config[0] == '[')
    {
        parseIPV6(config, server);
    }
    else
    {
        parseIPV4(config, server);
    }
    nextWord = getNextWord(config);
    if (nextWord != ";")
        throw Exception("Listen directive is not terminated by: \";\"");
    config.erase(0, 1);
}

void parseErrorPageDirective(std::string &config, ServerConfig &server)
{
    Utils::trimStart(config);
    size_t error_page_directive_len = config.find_first_of(";");
    if (error_page_directive_len == std::string::npos)
        throw Exception("Unclosed error_page directive, expecting \";\" ");
    std::string fullValue = config.substr(0, error_page_directive_len);
    config.erase(0, error_page_directive_len + 1);
    std::vector<std::string> arguments = Utils::split(fullValue, "\n \t");
    if (arguments.size() <= 1)
        throw Exception("Invalid arguments at error_page directive");
    std::string path = arguments[arguments.size() - 1];
    for (size_t i = 0; i < arguments.size(); i++)
    {
        if (i == (arguments.size() - 1))
            break;
        if (arguments[i].find_first_not_of("0123456789") != std::string::npos)
            throw Exception("Invalid status at error_page directive, must be only numeric");
        int status = std::atoi(arguments[i].c_str());
        if (status < 300 || status > 599 || arguments[i].size() > 3)
            throw Exception("Invalid status at error_page directive, must be more than 300, and less than 599");
        server.addErrorPage(status, path);
    }
}

std::string getSimpleDirectiveValue(std::string &config)
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
            parseListenDirective(config, server);
        else if (nextWord == "location")
            parseLocationDirective(config, server);
        else if (nextWord == "server_name")
            server.setServerName(getSimpleDirectiveValue(config));
        else if (nextWord == "client_max_body_size")
        {
            server.setMaxBodySize(getSimpleDirectiveValue(config));
            if (!isValidSizeValue(server.getMaxBodySize()))
                throw Exception("Invalid prop: \"" + server.getMaxBodySize() + "\" in client_max_body_size");
        }
        else if (nextWord == "error_page")
            parseErrorPageDirective(config, server);
        else if (nextWord == "}" || nextWord.empty())
            break;
        else
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" + Utils::concatConstChars(validServerProps) + "\"");
    }
    if (nextWord != "}")
        throw Exception("Server directive is unclosed, expecting: \"}\"");
    this->addServerConfig(server);
}

void Parser::parse()
{
    std::string str = this->_rawFile;

    std::string nextWord = getNextWord(str);
    while (!nextWord.empty())
    {
        if (nextWord != "server")
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"server\"");
        parseServerDirective(str);
        nextWord = getNextWord(str);
    }
}

void Parser::addServerConfig(ServerConfig newServer)
{
    this->_serversList.push_back(newServer);
}

std::vector<ServerConfig> Parser::getServerList()
{
    return this->_serversList;
}