#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <sstream>
#include "./Webcontent.hpp"
#include "./directives/ServerDirective.hpp"

class Server {
   private:
    int _socketFd;
    const ServerDirective& _config;

    // just for the sake of orthodox form, not really needed
    Server();
    Server(const Server& other);
    Server& operator=(const Server& other);

   public:
   Server(const ServerDirective& config);
    ~Server();

    void start();
    void stop();
    int getSocketFd() const;
    void acceptClient();

	void		contentManager(int clientFd);
	std::pair<std::string, std::string>	parseRequestedFile(const std::string& request);
	std::string	getMimeType(const std::string& fileName);
	std::string	buildHttpResponse(std::string fileContent, std::string contentType, bool error);	
	std::string	readFiles(const std::string& filePath);
	//void	Server::handlePOSTRequest();TODO
};

#endif  // SERVER_HPP
