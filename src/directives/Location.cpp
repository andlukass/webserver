#include "../../includes/directives/Location.hpp"

const char *validLocationProps[] = {"root", "index", "autoindex", "allow_methods", NULL};

Location::Location() : Directive("location") {
    // std::cout << "Location default constructor ===============================index: " <<
    // std::endl;
}

void deleteProps(DirectiveMap value) {
    for (size_t i = 0; validLocationProps[i]; i++) delete value[validLocationProps[i]];
}

Location::~Location() {
    // std::cout << "Location destructor ===============================index: " << std::endl;
    for (int i = 0; i < this->_value.size(); i++) {
        delete this->_value[i]["path"];
        deleteProps(this->_value[i]);
    }
}

Location::Location(const Location &other) : Directive(other._name) {
    // std::cout << "Location copy constructor ===============================index: " << std::endl;
    for (int i = 0; i < other._value.size(); i++) {
        this->_value.push_back(DirectiveMap());
        this->_value[i]["path"] = other._value[i].at("path")->clone();
        for (size_t j = 0; validLocationProps[j]; j++)
            this->_value[i][validLocationProps[j]] =
                other._value[i].at(validLocationProps[j])->clone();
    }
}

void Location::init() {
    // std::cout << "Location INIT ======================================= " << std::endl;
    this->_value.push_back(DirectiveMap());
    size_t index = this->_value.size() - 1;
    this->_value[index]["allow_methods"] = new AllowMethods();
    this->_value[index]["autoindex"] = new Autoindex();
    this->_value[index]["index"] = new MultiDirective("index");
    this->_value[index]["root"] = new Root();
    this->_value[index]["path"] = new Path();
}

void Location::parse(std::string &config) {
    this->init();
    size_t index = this->_value.size() - 1;

    this->_value[index]["path"]->parse(config);
    std::string initiator = getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\"");

    std::string nextWord = this->getNextWord(config);
    while (!nextWord.empty() && nextWord != "}") {
        if (!this->_value[index][nextWord] || nextWord == "path")
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" +
                            Utils::concatConstChars(validLocationProps) + "\"");
        this->_value[index][nextWord]->parse(config);
        nextWord = this->getNextWord(config);
    }
    if (nextWord != "}") throw Exception("Location directive is unclosed, expecting: \"}\"");
}

void printProps(DirectiveMap value) {
    for (size_t i = 0; validLocationProps[i]; i++) {
        std::cout << "\t";
        value[validLocationProps[i]]->print();
    }
}

void Location::print() const {
    std::cout << "Location: " << std::endl;
    for (int i = 0; i < this->_value.size(); i++) {
        this->_value[i].at("path")->print();
        printProps(this->_value[i]);
    }
}

const std::vector<DirectiveMap> &Location::getValue() const { return this->_value; }

Location *Location::clone() const { return new Location(*this); }
