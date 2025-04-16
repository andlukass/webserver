#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <map>
#include <string>

class CgiHandler {
   public:
    CgiHandler(const std::string& scriptPath);
    ~CgiHandler();

    std::string execute();

   private:
    std::string _scriptPath;
};

#endif  // CGI_HANDLER_HPP
