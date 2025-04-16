#include "../../includes/cgi/CgiHandler.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <stdio.h>


CgiHandler::CgiHandler(const std::string& scriptPath) : _scriptPath(scriptPath) {}

CgiHandler::~CgiHandler() {}

std::string CgiHandler::execute() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return "Error creating pipe";
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return "Error forking process";
    }

    // TODO: remember to add validations for not existing file
    // TODO: pass env variables
    // TODO: check for fd leaks before delivering
    if (pid == 0) {
        // CHILD
        close(fd[0]);                // close read end
        dup2(fd[1], STDOUT_FILENO);  // stdout → to pipe
        close(fd[1]);                // close not needed fd

        char* const args[] = {(char*)"python3", (char*)_scriptPath.c_str(), NULL};
        char* const env[] = {NULL};  // no variables at the moment

        execve("/usr/bin/python3", args, env);
        perror("execve");  // this is case where execve did not take over
        exit(1);
    } else {
        // PARENT
        close(fd[1]);  // close write end
        char buffer[1024];
        std::string result;
        ssize_t bytesRead;

        while ((bytesRead = read(fd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }

        close(fd[0]);
        waitpid(pid, NULL, 0);  // wait for child to finish

        return result;
    }
}
