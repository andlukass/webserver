#include "../../includes/directives/LocationDirective.hpp"

const char *validLocationProps[] = {"root", "index", "autoindex", "allow_methods", "error_page", NULL};

LocationDirective::LocationDirective() : Directive("server") {
    // std::cout << "LocationDirective default constructor ========================================"
    // << std::endl;
    this->_value["root"] = new Root();
    this->_value["path"] = new Path();
    this->_value["index"] = new Index();
    this->_value["autoindex"] = new Autoindex();
    this->_value["error_page"] = new ErrorPage();
    this->_value["allow_methods"] = new AllowMethods();
}

LocationDirective::LocationDirective(const LocationDirective &other) : Directive(other._name) {
    // std::cout << "LocationDirective copy constructor ========================================" <<
    // std::endl;
    this->_value["path"] = other._value.at("path")->clone();
    for (size_t i = 0; validLocationProps[i]; i++)
        this->_value[validLocationProps[i]] = other._value.at(validLocationProps[i])->clone();
}

LocationDirective::~LocationDirective() {
    // std::cout << "LocationDirective destructor ========================================" <<
    // std::endl;

    delete this->_value["path"];
    for (size_t i = 0; validLocationProps[i]; i++) {
        // std::cout << "deleting " << validLocationProps[i] << std::endl;
        delete this->_value[validLocationProps[i]];
    }
}

LocationDirective &LocationDirective::operator=(const LocationDirective &other) {
    if (this != &other) {
        this->_value["path"] = other._value.at("path")->clone();
        for (size_t i = 0; validLocationProps[i]; i++)
            this->_value[validLocationProps[i]] = other._value.at(validLocationProps[i])->clone();
    }
    return *this;
}

void LocationDirective::parse(std::string &config) {
    this->_value["path"]->parse(config);
    std::string initiator = getNextWord(config);
    if (initiator != "{")
        throw Exception("Unexpected token: \"" + initiator + "\" expecting: \"{\"");

    std::string nextWord = this->getNextWord(config);
    while (!nextWord.empty() && nextWord != "}") {
        if (!this->_value[nextWord])
            throw Exception("Unexpected prop: \"" + nextWord + "\" expecting: \"" +
                            Utils::concatConstChars(validLocationProps) + "\"");
        this->_value[nextWord]->parse(config);
        nextWord = this->getNextWord(config);
    }
    if (nextWord != "}") throw Exception("Server directive is unclosed, expecting: \"}\"");
}

void LocationDirective::print() const {
    std::cout << "Location: " << std::endl;
    this->_value.at("path")->print();
    for (size_t i = 0; validLocationProps[i]; i++) {
        std::cout << "\t";
        this->_value.at(validLocationProps[i])->print();
    }
}

LocationDirective *LocationDirective::clone() const { return new LocationDirective(*this); }

Autoindex *LocationDirective::getAutoindex() const {
    return dynamic_cast<Autoindex *>(this->_value.at("autoindex"));
}

AllowMethods *LocationDirective::getAllowMethods() const {
    return dynamic_cast<AllowMethods *>(this->_value.at("allow_methods"));
}

Root *LocationDirective::getRoot() const { return dynamic_cast<Root *>(this->_value.at("root")); }

Path *LocationDirective::getPath() const { return dynamic_cast<Path *>(this->_value.at("path")); }

Index *LocationDirective::getIndex() const { return dynamic_cast<Index *>(this->_value.at("index")); }

ErrorPage *LocationDirective::getErrorPage() const { return dynamic_cast<ErrorPage *>(this->_value.at("error_page")); }