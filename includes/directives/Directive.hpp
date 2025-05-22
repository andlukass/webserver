#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include <cstdlib>
#include <iostream>
#include <string>

#include "../Exception.hpp"
#include "../Utils.hpp"

class Directive {
   public:
    Directive(const std::string &name) : _name(name) {
        // std::cout << "Directive default constructor ========================================" <<
        // std::endl;
    }
    virtual ~Directive() {
        // std::cout << "Directive destructor ========================================" <<
        // std::endl;
    }

    virtual void parse(std::string &config) = 0;
    virtual void print() const = 0;
    virtual Directive *clone() const = 0;
    static std::string getNextWord(std::string &config);

   protected:
    const std::string _name;
    std::string getFullValue(std::string &config, char closeChar);
};

typedef std::map<std::string, Directive *> DirectiveMap;

#endif