/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webcontent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngtina1999 <ngtina1999@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:51:23 by ngtina1999        #+#    #+#             */
/*   Updated: 2025/04/14 16:53:40 by ngtina1999       ###   ########.fr       */
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
        return "index.html";  // Default file
    }

    return filePath.substr(1);  // Remove the leading "/"
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
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << "Content-Length: " << fileContent.size() << "\r\n"
             << "Connection: close\r\n\r\n"
             << fileContent;
	return(response.str());
}

std::string Webcontent::readFiles(const std::string& filePath) {
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);

    // Read file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    
    file.close();  // Close the file after reading
    return fileContent;  // Return file content
}

void	Webcontent::contentManager(int clientFd, std::string root) {
	
	//it might be better instead of memory alloc, it is not working with more than 1024 char though
	char buffer[1024];
    ssize_t httpHeader = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (httpHeader <= 0) {
        close(clientFd); //mabye it is unnecessary since Yulia already close the clienFD in ServerManager
        return;
    }

    buffer[httpHeader] = '\0';
    std::string request(buffer);

    // Extract requested file from the GET request
    std::string fileName = parseRequestedFile(request);
    if (fileName.empty()) {
        fileName = "index.html";  // default to index.html if no file is specified
    }

    // Read the requested file
    std::string filePath = root + fileName;
	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!file) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
		filePath = "./src/webcontent/errorpage.html";
	}

    std::string fileContent = readFiles(filePath);

	    // std::ifstream file("./src/webcontent/webcontent.html", std::ios::in | std::ios::binary);
    // if (!file) {
    //     std::cerr << "Error: Could not open HTML file" << std::endl;
    //     close(clientFd);
    //     return;
    // }
    // std::string htmlContent((std::istreambuf_iterator<char>(file)),
    //                         std::istreambuf_iterator<char>());
    // Determine content type
    std::string contentType = getMimeType(fileName);

    // Build and send response
    std::string response = buildHttpResponse(fileContent, contentType);
    send(clientFd, response.c_str(), response.size(), 0);

    std::cout << "Sent file: " << filePath << std::endl;

	// std::cout << "HTML content sent to client!" << std::endl;

}