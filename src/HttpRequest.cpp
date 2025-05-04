#include "../includes/HttpRequest.hpp"

#include <sstream>

enum ErrorStatus {
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    BAD_REQUEST = 400,
};

std::string statusToString(int errorStatus) {
    switch (errorStatus) {
        case NOT_FOUND:
            return "404 Not Found";
        case METHOD_NOT_ALLOWED:
            return "405 Method Not Allowed";
        case BAD_REQUEST:
            return "400 Bad Request";
        default:
            return "";
    }
}

void HttpRequest::buildErrorResponse(int errorStatus) {
    std::cout << "Building error response" << std::endl;
    std::stringstream response;
    std::string fileContent = Utils::readFile("./src/webcontent/errorpage");
    response << "HTTP/1.1 " << errorStatus << " " << statusToString(errorStatus) << "\r\n";
    if (errorStatus == NOT_FOUND) {
        if (fileContent.empty()) {
            fileContent = "<html><body><h1>"+ statusToString(errorStatus) + "</h1></body></html>";
        }
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << fileContent.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << fileContent;
    }
    if (errorStatus == BAD_REQUEST) {
        fileContent = "<html><body><h1>" + statusToString(errorStatus) + "</h1></body></html>";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << fileContent.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << fileContent;
    }
    if (errorStatus == METHOD_NOT_ALLOWED) {
        fileContent = "<html><body><h1>" + statusToString(errorStatus) + "</h1></body></html>";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << fileContent.size() << "\r\n";
        response << "Connection: close\r\n\r\n";
        response << fileContent;
    }
    this->_response = response.str();
    this->_status = errorStatus;
}

void HttpRequest::buildOKResponse(std::string fileContent, std::string contentType) {
    // std::cout << "TESTE DO LOCATION" << this->_config.getLocation("/root").getPath()->getValue() << std::endl;
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << fileContent.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << fileContent;
    this->_response = response.str();
    this->_status = 200;
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

HttpRequest::HttpRequest(const ServerDirective& config, const std::string& request)
    : _rawRequest(request),
      _method(METHOD_UNKNOWN),
      _httpVersion(HTTP_VERSION_UNKNOWN),
      _cgiType(CGI_NONE),
      _isCgi(false),
      _config(config) {
            this->initFromRaw();
            // std::cout << "THIS IS THE REQUEST " << request.getRawUri() << std::endl;
      }

HttpRequest::~HttpRequest() {}

HttpMethod parseMethod(const std::string methodString) {
    if (methodString == "GET")
        return METHOD_GET;
    else if (methodString == "POST")
        return METHOD_POST;
    else if (methodString == "DELETE")
        return METHOD_DELETE;
    else
        return METHOD_UNKNOWN;
}

HttpVersion parseHttpVersion(const std::string httpVersionString) {
    if (httpVersionString == "HTTP/1.1")
        return HTTP_1_1;
    else if (httpVersionString == "HTTP/1.0")
        return HTTP_1_0;
    else
        return HTTP_VERSION_UNKNOWN;
}

bool HttpRequest::parseRequestLine() {
    size_t firstLineEnd = _rawRequest.find("\r\n");
    if (firstLineEnd == std::string::npos) return false;
    size_t headersEnd = _rawRequest.find("\r\n\r\n");
    if (headersEnd == std::string::npos) return false;
    size_t headersStart = firstLineEnd + 2;


    std::string startLine = _rawRequest.substr(0, firstLineEnd);

    std::istringstream iss(startLine);
    std::string methodStr, uriStr, versionStr;
    iss >> methodStr >> uriStr >> versionStr;

    if (methodStr.empty() || uriStr.empty() || versionStr.empty()) {
        return false;
    }

    _method = parseMethod(methodStr);
    _cleanUri = Utils::cleanSlashes(uriStr);
    _httpVersion = parseHttpVersion(versionStr);
    _rawHeaders = _rawRequest.substr(headersStart, headersEnd);

    if (_method == METHOD_UNKNOWN || _httpVersion == HTTP_VERSION_UNKNOWN) {
        return false;
    }
    return true;
}

void HttpRequest::parseHeaders() {
    std::istringstream stream(_rawHeaders);
    std::string line;

    while (std::getline(stream, line)) {
        // to remove \r in the end of the line
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        size_t colonPos = line.find(':');
        // if header doesn't have : in the middle, or as first or last symbol - skip it
        if (colonPos == std::string::npos || colonPos == 0 || colonPos == line.size()) {
            continue;
        }

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        size_t firstNotSpace = value.find_first_not_of(" \t");
        if (firstNotSpace != std::string::npos) {
            value = value.substr(firstNotSpace);
        } else
            value = "";

        _headers[key] = value;
    }
}

void HttpRequest::parseBody() {
    size_t bodyStart = _rawRequest.find("\r\n\r\n");
    // checking if body is present. we don't validate if it's supposed to be at the moment
    if (bodyStart != std::string::npos) {
        // +4 here to skip empty line in between headers and body
        bodyStart += 4;
        if (bodyStart < _rawRequest.size()) _body = _rawRequest.substr(bodyStart);
    }
}

void HttpRequest::parseIndex() {
    char lastChar = _cleanUri.back();
    bool hasFile = lastChar != '/';

    std::string serverIndex =  _locationPath.empty() ? "" : _config.getLocation(_locationPath).getIndex()->getValue();
    if (serverIndex.empty()) {
        serverIndex = _config.getIndex()->getValue();
    }
    if (serverIndex.empty()) {
        serverIndex = "/index.html";
    }
    _index = hasFile ? "" : serverIndex;
}

void HttpRequest::parseRoot() {
    std::string serverRoot = _locationPath.empty() ? "" : _config.getLocation(_locationPath).getRoot()->getValue();
    if (serverRoot.empty()) {
        serverRoot = _config.getRoot()->getValue();
    }
    if (serverRoot.empty()) {
        serverRoot = "./";
    }
    _root = serverRoot;
}

void HttpRequest::parseLocation() {
    char lastChar = _cleanUri.back();
    bool hasFile = lastChar != '/';
    std::string locationPath = _cleanUri;

    if (hasFile) {
        size_t lastSlash = _cleanUri.find_last_of('/');
        if (lastSlash != std::string::npos) {
            locationPath = _cleanUri.substr(0, lastSlash + 1);
        }
    }

    try {
        _config.getLocation(locationPath);
        _locationPath = locationPath;
    } catch (const std::exception &e) {
        _locationPath = "";
    }
}

void HttpRequest::detectCgiAndMime() {
    size_t dotPos = _cleanUri.rfind('.');
    if (dotPos != std::string::npos) {
        std::string ext = _cleanUri.substr(dotPos);

        if (ext == ".py") {
            _isCgi = true;
            _cgiType = CGI_PYTHON;
        } else {
            _isCgi = false;
            _cgiType = CGI_NONE;
        }

        if (ext == ".html")
            _mimeType = "text/html";
        else if (ext == ".css")
            _mimeType = "text/css";
        else if (ext == ".js")
            _mimeType = "application/javascript";
        else if (ext == ".jpg")
            _mimeType = "image/jpeg";
        else if (ext == ".png")
            _mimeType = "image/png";
        else
            _mimeType = "text/html";

    } else {
        _isCgi = false;
        _cgiType = CGI_NONE;
        _mimeType = "text/html";
    }
}

void HttpRequest::parseResponse() {
    const std::vector<std::string>& allowedMethods = 
        _config.getAllowMethods()->getValue();
    std::string strMethod = methodToString(_method);


    if (std::find(allowedMethods.begin(), allowedMethods.end(), strMethod) == allowedMethods.end()) {
        std::cerr << "Method not allowed: " << strMethod << std::endl;
        this->buildErrorResponse(METHOD_NOT_ALLOWED);
        return;
    }

    std::string filePath = _root + _cleanUri + _index;
    std::cout << "TESTE DO _cleanUri" << filePath << std::endl;
    std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        // TODO [CGI] - now it returns error, because we can't open HTML with that. we can fix later
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        this->buildErrorResponse(NOT_FOUND);
        return;
    }

    if (_isCgi) {
        std::cout << "[CGI] Detected Python Script " << filePath << std::endl;

        CgiHandler cgi(filePath);
        std::string cgiResult = cgi.execute();
        // I wrap CGI response in HTML style now
        this->buildOKResponse(cgiResult, "text/html");
        // std::cout << "[CGI] Returned output\n" << cgiResult << std::endl;
        // send(clientFd, cgiResult.c_str(), cgiResult.size(), 0);
        // this->close();
        return;
    }

    std::string fileContent = Utils::readFile(filePath);
    std::string contentType = _mimeType;  // we need these two to send the content

    this->buildOKResponse(fileContent, contentType);
}

void HttpRequest::initFromRaw() {
    if (!parseRequestLine()) {
        this->buildErrorResponse(BAD_REQUEST);
        return;
    }
    parseLocation();
    parseIndex();
    parseRoot();
    parseHeaders();
    parseBody();
    detectCgiAndMime();
    parseResponse();
}

HttpMethod HttpRequest::getMethod() const { return _method; };
std::string HttpRequest::getRawRequest() const { return _rawRequest; };
std::string HttpRequest::getCleanUri() const { return _cleanUri; };
// std::string HttpRequest::getPath() const { return _path; };
std::map<std::string, std::string> HttpRequest::getQueryParams() const { return _queryParams; };
HttpVersion HttpRequest::getHttpVersion() const { return _httpVersion; };
std::map<std::string, std::string> HttpRequest::getHeaders() const { return _headers; };
std::string HttpRequest::getBody() const { return _body; };
std::string HttpRequest::getMimeType() const { return _mimeType; };
CgiType HttpRequest::getCgiType() const { return _cgiType; };
bool HttpRequest::getIsCgi() const { return _isCgi; };
std::string HttpRequest::getResponse() const { return _response; };