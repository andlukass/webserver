#include "../../includes/directives/ServerDirective.hpp"

const char *validServerProps[] = {"listen", "server_name", "client_max_body_size", "index", "root", "error_page", "location", NULL};

ServerDirective::ServerDirective() : Directive("server")
{
    // std::cout << "ServerDirective default constructor ========================================" << std::endl;
    this->_value["listen"] = new Listen();
    this->_value["server_name"] = new MultiDirective("server_name");
    this->_value["client_max_body_size"] = new ClientMaxBodySize();
    this->_value["index"] = new MultiDirective("index");
    this->_value["root"] = new Root();
    this->_value["error_page"] = new ErrorPage();
    this->_value["location"] = new Location();
}

ServerDirective::ServerDirective(const ServerDirective &other) : Directive(other._name)
{
    std::cout << "ServerDirective copy constructor ========================================" << std::endl;
    for (size_t i = 0; validServerProps[i]; i++)
        this->_value[validServerProps[i]] = other._value.at(validServerProps[i])->clone();
}

ServerDirective::~ServerDirective()
{
    // std::cout << "ServerDirective destructor ========================================" << std::endl;

    for (size_t i = 0; validServerProps[i]; i++)
    {
        // std::cout << "deleting " << validServerProps[i] << std::endl;
        delete this->_value[validServerProps[i]];
    }
}

void ServerDirective::parse(std::string &config)
{
    std::string initiator = this->getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\". At " + this->_name + " directive");

    std::string nextWord = this->getNextWord(config);
    while (!nextWord.empty() && nextWord != "}")
    {
        if (!this->_value[nextWord])
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" + Utils::concatConstChars(validServerProps) + "\"");
        this->_value[nextWord]->parse(config);
        nextWord = this->getNextWord(config);
    }
    if (nextWord != "}")
        throw Exception("Server directive is unclosed, expecting: \"}\"");
}

void ServerDirective::print()
{
    for (size_t i = 0; validServerProps[i]; i++)
        this->_value[validServerProps[i]]->print();
}

std::map<std::string, Directive *> ServerDirective::getValue() const
{
    return this->_value;
}

ServerDirective *ServerDirective::clone() const
{
    return new ServerDirective(*this);
}