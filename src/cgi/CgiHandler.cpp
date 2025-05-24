#include "../../includes/cgi/CgiHandler.hpp"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <sstream>

CgiHandler::CgiHandler(const std::string& scriptPath) : _scriptPath(scriptPath) {}

CgiHandler::~CgiHandler() {}

std::string cgiFail(const std::string& msg) {
    std::cerr << "[CGI ERROR] " << msg << std::endl;
    return CGI_ERROR_RESPONSE;
}

std::string CgiHandler::execute(const std::string& body, const std::string& method,
                                const int cgi_type) {
    int stdoutPipe[2];
    int stdinPipe[2];

    if (pipe(stdoutPipe) == -1 || pipe(stdinPipe) == -1) {
        perror("pipe");
        cgiFail("Error creating pipes");
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        cgiFail("Error forking process");
    }

    if (pid == 0) {
        // CHILD
        dup2(stdinPipe[0], STDIN_FILENO);
        dup2(stdoutPipe[1], STDOUT_FILENO);

        close(stdinPipe[1]);
        close(stdoutPipe[0]);

        std::ostringstream oss;
        oss << body.size();
        std::string contentLength = oss.str();

        std::string methodEnv = "REQUEST_METHOD=" + method;
        std::string lengthEnv = "CONTENT_LENGTH=" + contentLength;

        char* const env[] = {(char*)methodEnv.c_str(), (char*)lengthEnv.c_str(),
                             (char*)"CONTENT_TYPE=application/x-www-form-urlencoded", NULL};

        // char* const args[] = {(char*)_scriptPath.c_str(), NULL};

        if (cgi_type == CGI_PYTHON) {
            char* const pyArgs[] = {(char*)"python3", (char*)_scriptPath.c_str(), NULL};
            execve("/usr/bin/python3", pyArgs, env);
        } else if (cgi_type == CGI_BASH) {
            char* const shArgs[] = {(char*)"bash", (char*)_scriptPath.c_str(), NULL};
            execve("/bin/bash", shArgs, env);
        } else {
            std::cerr << "[CGI ERROR] Unsupported CGI type" << std::endl;
            exit(1);  // exit child
        }

        perror("execve");
        exit(1);
    } else {
        // PARENT
        close(stdinPipe[0]);
        close(stdoutPipe[1]);

        // request body
        ssize_t written = write(stdinPipe[1], body.c_str(), body.size());
        if (written < 0) {
            std::cerr << "[CGI] Error writing to stdin pipe" << std::endl;
        } else if (written == 0) {
            std::cerr << "[CGI] Warning: write() wrote 0 bytes to stdin pipe" << std::endl;
        }
        close(stdinPipe[1]);

        int status;
        int elapsed = 0;
        pid_t waitResult;

        while ((waitResult = waitpid(pid, &status, WNOHANG)) == 0 &&
               elapsed < CGI_TIMEOUT_SECONDS) {
            sleep(1);
            // debugging timeout - if needed
            // std::cout << "Elapsed: " << elapsed << std::endl;
            elapsed++;
        }

        if (waitResult == 0) {
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            return cgiFail("CGI timeout - killed");
        }

        if (waitResult == -1) {
            return cgiFail("waitpid failed");
        }

        // This is the case when child exited with 1 (execve failed)
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            return cgiFail("CGI process exited abnormally or with error");
        }

        // reading response
        char buffer[1024];
        std::string result;
        ssize_t bytesRead;
        while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }

        if (bytesRead < 0) {
            std::cerr << "[CGI] Error reading from CGI stdout pipe" << std::endl;
        } else if (bytesRead == 0 && result.empty()) {
            std::cerr << "[CGI] Warning: read() returned 0, CGI output is empty" << std::endl;
        }

        close(stdoutPipe[0]);

        return result;
    }
}
