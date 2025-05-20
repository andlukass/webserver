#include "../../includes/cgi/CgiHandler.hpp"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

CgiHandler::CgiHandler(const std::string& scriptPath) : _scriptPath(scriptPath) {}

CgiHandler::~CgiHandler() {}

std::string cgiFail(const std::string& msg) {
    std::cerr << "[CGI ERROR]" << msg << std::endl;
    return CGI_ERROR_RESPONSE;
}

std::string CgiHandler::execute(const std::string& body, const std::string& method) {
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

        std::string contentLength = std::to_string(body.size());

        std::string methodEnv = "REQUEST_METHOD=" + method;
        std::string lengthEnv = "CONTENT_LENGTH=" + contentLength;

        char* const env[] = {(char*)methodEnv.c_str(), (char*)lengthEnv.c_str(),
                             (char*)"CONTENT_TYPE=application/x-www-form-urlencoded", NULL};

        char* const args[] = {(char*)"python3", (char*)_scriptPath.c_str(), NULL};

        execve("/usr/bin/python3", args, env);
        perror("execve");
        exit(1);
    } else {
        // PARENT
        close(stdinPipe[0]);
        close(stdoutPipe[1]);

        // request body
        write(stdinPipe[1], body.c_str(), body.size());
        close(stdinPipe[1]);

        // reading response
        char buffer[1024];
        std::string result;
        ssize_t bytesRead;
        while ((bytesRead = read(stdoutPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }

        close(stdoutPipe[0]);

        int status;
        int elapsed = 0;
        pid_t waitResult;

        while ((waitResult = waitpid(pid, &status, WNOHANG)) == 0 &&
               elapsed < CGI_TIMEOUT_SECONDS) {
            sleep(1);
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

        return result;
    }
}
