/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webcontent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngtina1999 <ngtina1999@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:51:23 by ngtina1999        #+#    #+#             */
/*   Updated: 2025/04/14 17:34:38 by ngtina1999       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webcontent.hpp"

Webcontent::Webcontent() {
}

Webcontent::~Webcontent() {
}

Webcontent::Webcontent(const Webcontent &copy) {
	(void)copy;
}

Webcontent & Webcontent::operator=(const Webcontent &rhs) {
	(void)rhs;
	return(*this);
}


std::string Webcontent::parseRequestedFile(const std::string& request) {
    size_t start = request.find("GET ") + 4;
    size_t end = request.find(" ", start);
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }

    std::string filePath = request.substr(start, end - start);
    if (filePath == "/" || filePath.empty()) {
        return "index.html";  // default file
    }

    return filePath.substr(1);  // remove the leading "/" so not from the 0
}

std::string Webcontent::getMimeType(const std::string& fileName) {
    if (fileName.find(".html") != std::string::npos)//I'm not sure what we need here 
		return ("text/html");
	else if (fileName.find(".css") != std::string::npos)
		return ("text/css");
    else if (fileName.find(".js") != std::string::npos)
		return ("application/javascript");
    else if (fileName.find(".jpg") != std::string::npos)
		return ("image/jpeg");
    else if (fileName.find(".png") != std::string::npos)
		return ("image/png");
	else
		return (".html");
    //return "text/plain";  // default to plain text, I think it should work with this
}

std::string Webcontent::buildHttpResponse(std::string fileContent, std::string contentType) {
	// Build HTTP response header
	// std::string response = "HTTP/1.1 200 OK\r\n";
	// response += "Content-Type: text/html\r\n";

	// // Convert size to string using stringstream (for C++98)
	// std::stringstream ss;
	// ss << fileContent.size();
	// std::string contentLength = ss.str();
	// response += "Content-Length: " + contentLength + "\r\n";

	// response += "Connection: close\r\n\r\n";  // Close connection after response
	// response += fileContent;

	std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n" // this is something connected to the error messages I think, but I do in a different way
             << "Content-Type: " << contentType << "\r\n"
             << "Content-Length: " << fileContent.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << fileContent;
	return(response.str());
}

std::string Webcontent::readFiles(std::ifstream& file) {
    // read file content into a string, maybe I should use this at the http header file
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    
    file.close();  // close the file after reading
    return (fileContent);  // Return file content
}

void	Webcontent::contentManager(int clientFd, std::string root) {
	
	//it might be better instead of memory alloc, it is not working with more than 1024 char though
	char buffer[1024];
    ssize_t httpHeader = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
	std::cout << httpHeader << std::endl;
    if (httpHeader <= 0) {
        close(clientFd); //mabye it is unnecessary since Yulia already close the clienFD in ServerManager
        return;
    }

    buffer[httpHeader] = '\0';
    std::string request(buffer);
	std::cout << "THIS IS THE REQUEST" << request << std::endl;
    // Extract requested file from the GET request
    std::string fileName = parseRequestedFile(request);
    if (fileName.empty()) {
        fileName = "index.html";  // default to index.html if no file is specified
    }

    // Read the requested file
    std::string filePath = root + fileName;
	//c_str ->C type contact char*, std::ios::in->default read input mode, std::ios::binar->
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!file) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
		filePath = "./src/webcontent/errorpage.html"; // it puts the error .html I'm not sure if it's the way to go, maybe there are different type of errors here to send
	}

	//std::cout << "Sent file: " << filePath << std::endl; for some reason it always shows the error path, but it's working
	
    std::string fileContent = readFiles(file);
    std::string contentType = getMimeType(fileName);// we need these two to send the content

    // build and send response
    std::string response = buildHttpResponse(fileContent, contentType);
	std::cout << "This is the HTTP response over the network to the client (here the browser) through the open socket connection: \n" <<
	response << std::endl;
    send(clientFd, response.c_str(), response.size(), 0);

	// std::cout << "HTML content sent to client!" << std::endl;

}