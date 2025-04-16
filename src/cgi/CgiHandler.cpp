#include "../../includes/cgi/CgiHandler.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

CgiHandler::CgiHandler(const std::string& scriptPath) : _scriptPath(scriptPath) {}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::execute() {
    std::cout << "Pretend we're launching: " << _scriptPath << std::endl;
    return "placeholder response from CGI";
}
