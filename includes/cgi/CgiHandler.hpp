#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <map>
#include <string>

class CgiHandler {
   public:
    CgiHandler(const std::string& scriptPath);
    ~CgiHandler();

    std::string execute(const std::string& body, const std::string& method);

   private:
    std::string _scriptPath;
};

#endif  // CGI_HANDLER_HPP
