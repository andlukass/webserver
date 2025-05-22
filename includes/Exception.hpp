#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class Exception : public std::exception {
   private:
    std::string _message;

   public:
    Exception() throw() { _message = "An error occurred"; }
    Exception(std::string message) throw() { _message = message; }
    virtual const char *what() const throw() { return (_message.c_str()); }
    virtual ~Exception() throw() {}
};

#endif