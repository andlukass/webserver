#include "../../includes/directives/Location.hpp"

const char *validLocationProps[] = {"root", "index", "autoindex", "allow_methods", NULL};

Location::Location() : Directive("location")
{
    this->_lastIndex = 0;
}

Location::~Location()
{
}

void Location::init()
{
    this->_value.push_back(LocationValue());
    this->_value[this->_lastIndex]["allow_methods"] = new AllowMethods();
    this->_value[this->_lastIndex]["autoindex"] = new Autoindex();
    this->_value[this->_lastIndex]["index"] = new MultiDirective("index");
    this->_value[this->_lastIndex]["root"] = new Root();
    this->_value[this->_lastIndex]["path"] = new Path();
}

void Location::parse(std::string &config)
{
    this->init();
    this->_value[this->_lastIndex]["path"]->parse(config);
    std::string initiator = getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\"");

    std::string nextWord = this->getNextWord(config);
    while (!nextWord.empty() && nextWord != "}")
    {
        if (!this->_value[this->_lastIndex][nextWord] || nextWord == "path")
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" + Utils::concatConstChars(validLocationProps) + "\"");
        this->_value[this->_lastIndex][nextWord]->parse(config);
        nextWord = this->getNextWord(config);
    }
    if (nextWord != "}")
        throw Exception("Location directive is unclosed, expecting: \"}\"");
    this->_lastIndex++;
}

void printProps(LocationValue value)
{
    for (size_t i = 0; validLocationProps[i]; i++)
    {
        std::cout << "\t";
        value[validLocationProps[i]]->print();
    }
}

void Location::print()
{
    std::cout << "Location: " << std::endl;
    for (int i = 0; i < this->_lastIndex; i++)
    {
        this->_value[i]["path"]->print();
        printProps(this->_value[i]);
    }
}

std::vector<LocationValue> Location::getValue() const
{
    return this->_value;
}
