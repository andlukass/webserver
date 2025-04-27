#include <algorithm>

#include "../includes/HttpRequest.hpp"
#include "../includes/Server.hpp"
#include "../includes/cgi/CgiHandler.hpp"

std::string Server::buildHttpResponse(std::string fileContent, std::string contentType,
                                      bool error) {
    std::stringstream response;

    if (error == true)
        response << "HTTP/1.1 404 Not Found\r\n";
    else
        response << "HTTP/1.1 200 OK\r\n";

    response
        << "Content-Type: " << contentType << "\r\n"
        << "Content-Length: " << fileContent.size() << "\r\n"
        << "Connection: close\r\n\r\n"  // Carriage Return (\r), Line Feed (\n): together represent
                                        // standard way of ending a line in HTTP headers
        << fileContent;

    return (response.str());
}

std::string Server::readFiles(const std::string& filePath) {
    std::ifstream file(
        filePath.c_str(),
        std::ios::in |
            std::ios::binary);  // it is needed because if it's error it has to read again

    // read file content into a string, maybe I should use this at the http header file
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    file.close();          // close the file after reading
    return (fileContent);  // Return file content
}

// TODO: our config parser could use enum HttpMethod
// for now this is just translating to text
std::string methodToString(HttpMethod method) {
    switch (method) {
        case METHOD_GET:
            return "GET";
        case METHOD_POST:
            return "POST";
        case METHOD_DELETE:
            return "DELETE";
        default:
            return "";
    }
}

void Server::contentManager(int clientFd) {
    // it might be better instead of memory alloc, it is not working with more than 1024 char though
    char buffer[1024];
    ssize_t receivedData = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    // std::cout << receivedData << std::endl;
    if (receivedData <= 0) {
        close(clientFd);  // mabye it is unnecessary since Yulia already close the clienFD in
                          // ServerManager. YULIA: I took out closing from ServerManager
        return;
    }

    buffer[receivedData] = '\0';
    HttpRequest request(buffer);
    request.initFromRaw();

    //std::cout << "THIS IS THE REQUEST " << request.getRawUri() << std::endl;

    std::string method = methodToString(request.getMethod());


    const std::vector<std::string>& allowedMethods =
        _config.getLocation(0).getAllowMethods()->getValue();
    if (std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end()) {
        std::cerr << "Method not allowed: " << method << std::endl;
        std::string errorResponse = "HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\n\r\n";
        send(clientFd, errorResponse.c_str(), errorResponse.size(), 0);
        return;
    }

	if (method == "POST") {
		std::cout << "THIS IS THE BODY " << request.getBody() << std::endl;
		
	}

	std::string fileName = request.getCleanUri();

    // YULIA: I moved this logic to HttpRequest::detectCgiAndMime

    // if (fileName.empty() || fileName == "/") {
    //     std::cerr << "HERE" << std::endl;
    //     fileName = "/index.html";  // default to index.html if no file is specified, but this is
    //     //
    //                                // anyway bring us there
    // }

    // Read the requested file
    std::string filePath = _config.getRoot()->getValue() + fileName;

    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    bool error = false;

    if (request.getIsCgi()) {
        std::cout << "[CGI] Detected Python Script " << fileName << std::endl;

        CgiHandler cgi(filePath);
        std::string cgiResult = cgi.execute();
        std::stringstream response;
        // I wrap CGI response in HTML style now
        response << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: text/html\r\n"
                 << "Content-Length: " << cgiResult.size() << "\r\n"
                 << "Connection: close\r\n\r\n"
                 << cgiResult;
        send(clientFd, response.str().c_str(), response.str().size(), 0);
        std::cout << "[CGI] Returned output\n" << cgiResult << std::endl;
        // send(clientFd, cgiResult.c_str(), cgiResult.size(), 0);
        close(clientFd);
        return;
    }

    if (!request.getIsCgi() && !file) {
        // TODO [CGI] - now it returns error, because we can't open HTML with that. we can fix later
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        filePath = "./src/webcontent/errorpage";  // it puts the error .html I'm not sure if it's
                                                  // the way to go, maybe there are different type
                                                  // of errors here to
        error = true;
    }

    // std::cout << "Sent file: " << filePath << std::endl; for some reason it always shows the
    // error path, but it's working

    std::string fileContent = readFiles(filePath);
    std::string contentType = request.getMimeType();  // we need these two to send the content

    // build and send response
    std::string response = buildHttpResponse(fileContent, contentType, error);
    // std::cout << "This is the HTTP response over the network to the client (here the browser) \\
    // through the open socket connection: \n" << response << std::endl;
    send(clientFd, response.c_str(), response.size(), 0);
    // YULIA: now ServerManager is not closing by itself
    close(clientFd);

    // std::cout << "HTML content sent to client!" << std::endl;
}