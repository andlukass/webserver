#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <map>
#include <string>

#include "../Constants.hpp"

#define CGI_TIMEOUT_SECONDS 5
#define CGI_ERROR_RESPONSE "500"

class CgiHandler {
   public:
    CgiHandler(const std::string& scriptPath);
    ~CgiHandler();

    std::string execute(const std::string& body, const std::string& method, const int cgi_type);

   private:
    std::string _scriptPath;
};

#endif  // CGI_HANDLER_HPP
