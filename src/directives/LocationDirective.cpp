#include "../../includes/directives/LocationDirective.hpp"

const char *validLocationProps[] = {"root", "index", "autoindex", "allow_methods", "error_page", "redirect", NULL};

LocationDirective::LocationDirective() : Directive("server") {
    // std::cout << "LocationDirective default constructor ========================================"
    // << std::endl;
    this->_value["root"] = new Root();
    this->_value["path"] = new Path();
    this->_value["index"] = new FilePathDirective("index");
    this->_value["autoindex"] = new Autoindex();
    this->_value["error_page"] = new ErrorPage();
    this->_value["allow_methods"] = new AllowMethods();
    this->_value["redirect"] = new FilePathDirective("redirect");
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
        // Clean up current values to avoid memory leak
        delete this->_value["path"];
        for (size_t i = 0; validLocationProps[i]; i++) {
            delete this->_value[validLocationProps[i]];
        }

        // Deep copy from other
        this->_value["path"] = other._value.at("path")->clone();
        for (size_t i = 0; validLocationProps[i]; i++) {
            this->_value[validLocationProps[i]] = other._value.at(validLocationProps[i])->clone();
        }
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

FilePathDirective *LocationDirective::getIndex() const { return dynamic_cast<FilePathDirective *>(this->_value.at("index")); }

FilePathDirective *LocationDirective::getRedirect() const { return dynamic_cast<FilePathDirective *>(this->_value.at("redirect")); }

ErrorPage *LocationDirective::getErrorPage() const { return dynamic_cast<ErrorPage *>(this->_value.at("error_page")); }