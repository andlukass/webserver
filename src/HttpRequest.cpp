#include "../includes/HttpRequest.hpp"

enum ErrorStatus {
    NOT_FOUND = 404,
    NOT_FOUND_DELETE = 600,
    METHOD_NOT_ALLOWED = 405,
    BAD_REQUEST = 400,
    PAYLOAD_TOO_LARGE = 413,
    NO_CONTENT = 204,
    INTERNAL_SERVER_ERROR = 500
};

std::string statusToString(int errorStatus) {
    switch (errorStatus) {
        case NOT_FOUND:
            return "404 Not Found";
        case NOT_FOUND_DELETE:
            return "404 Not Found";
        case METHOD_NOT_ALLOWED:
            return "405 Method Not Allowed";
        case BAD_REQUEST:
            return "400 Bad Request";
        case PAYLOAD_TOO_LARGE:
            return "413 Payload Too Large";
        case NO_CONTENT:
            return "204 No Content";
        case INTERNAL_SERVER_ERROR:
            return "500 Internal Server Error";
        default:
            return "NOT HANDLED ERROR";
    }
}

void HttpRequest::buildErrorResponse(int errorStatus) {
    std::stringstream response;
    parseErrorPagePath(errorStatus);
    std::string fileContent = Utils::readFile(this->_errorPagePath);
    response << "HTTP/1.1 " << errorStatus << " " << statusToString(errorStatus) << "\r\n";
    if (fileContent.empty() || errorStatus == NOT_FOUND_DELETE) {
        fileContent =
            "<html>\n"
            "<head><title>" +
            statusToString(errorStatus) +
            "</title></head>\n"
            "<body>\n"
            "<center><h1>" +
            statusToString(errorStatus) +
            "</h1></center>\n"
            "</body>\n"
            "</html>\n";
    }
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << fileContent.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << fileContent;
    this->_response = response.str();
}

void HttpRequest::parseErrorPagePath(int errorStatus) {
    std::map<int, std::string> errorPage;
    if (!_locationPath.empty()) {
        errorPage = _config.getLocation(_locationPath).getErrorPage()->getValue();
        if (errorPage.find(errorStatus) != errorPage.end()) {
            _errorPagePath = errorPage.at(errorStatus);
            return;
        }
    }
    errorPage = _config.getErrorPage()->getValue();
    if (errorPage.find(errorStatus) != errorPage.end()) {
        _errorPagePath = errorPage.at(errorStatus);
        return;
    }
    _errorPagePath = "";
}

void HttpRequest::buildOKResponse(std::string fileContent, std::string contentType) {
    std::stringstream response;
    if (!_redirect.empty()) {
        response << "HTTP/1.1 302 Found\r\n";
        response << "Location: " << _redirect << "\r\n";
        response << "Connection: close\r\n\r\n";
        this->_response = response.str();
        std::cout << "response: " << this->_response << std::endl;
        return;
    }
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << fileContent.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << fileContent;
    this->_response = response.str();
}

void HttpRequest::buildAutoindexResponse(std::string filePath) {
    std::string pathToIndex = this->_locationPath + this->_cleanUri;
    std::stringstream htmlContent;
    htmlContent << "<html>" << std::endl;
    htmlContent << "<head>" << std::endl;
    htmlContent << "<title>Index of " << pathToIndex << "</title>" << std::endl;
    htmlContent << "<style>" << std::endl;
    htmlContent << "body { font-family: Arial, sans-serif; margin: 20px; }" << std::endl;
    htmlContent << "h1 { color: #333; }" << std::endl;
    htmlContent << "ul { list-style-type: none; padding: 0; }" << std::endl;
    htmlContent << "li { margin: 5px 0; }" << std::endl;
    htmlContent << "a { text-decoration: none; color: #0066cc; }" << std::endl;
    htmlContent << "a:hover { text-decoration: underline; }" << std::endl;
    htmlContent << "</style>" << std::endl;
    htmlContent << "</head>" << std::endl;
    htmlContent << "<body>" << std::endl;
    htmlContent << "<h1>Index of " << pathToIndex << "</h1>" << std::endl;
    htmlContent << "<ul>" << std::endl;

    if (pathToIndex != "/") {
        std::string parentDir = Utils::removeLastPathLevel(pathToIndex);
        htmlContent << "<li><a href=\"" << Utils::cleanSlashes(parentDir) << "\">../</a></li>"
                    << std::endl;
    }

    DIR* dir;
    struct dirent* entry;

    dir = opendir(filePath.c_str());
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            std::string name = entry->d_name;

            if (name == "." || name == "..") {
                continue;
            }

            std::string fullPath = filePath;
            if (fullPath[fullPath.size() - 1] != '/') fullPath += '/';
            fullPath += name;

            bool isDir = Utils::isDirectory(fullPath);
            std::string href = _locationPath + _cleanUri + "/" + name;
            if (isDir) {
                href += "/";
                htmlContent << "<li><a href=\"" << Utils::cleanSlashes(href) << "\">" << name
                            << "/</a></li>" << std::endl;
            } else {
                htmlContent << "<li><a href=\"" << Utils::cleanSlashes(href) << "\">" << name
                            << "</a></li>" << std::endl;
            }
        }
        closedir(dir);
    } else {
        htmlContent << "<li>Erro ao abrir o diretório</li>" << std::endl;
    }

    htmlContent << "</ul>" << std::endl;
    htmlContent << "</body>" << std::endl;
    htmlContent << "</html>" << std::endl;

    std::string fileContent = htmlContent.str();
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << fileContent.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << fileContent;
    this->_response = response.str();
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
    char lastChar = _cleanUri[_cleanUri.size() - 1];
    bool hasFile = !_cleanUri.empty() && lastChar != '/';

    std::string tempIndex =
        _locationPath.empty() ? "" : _config.getLocation(_locationPath).getIndex()->getValue();
    if (tempIndex.empty()) {
        tempIndex = _config.getIndex()->getValue();
    }
    if (tempIndex.empty()) {
        tempIndex = DEFAULT_INDEX;
    }

    std::string filePath = _root + _cleanUri + tempIndex;
    if (!Utils::isFile(filePath)) {
        tempIndex = "";
    }
    this->_index = hasFile ? "" : tempIndex;
}

void HttpRequest::parseRoot() {
    std::string tempRoot =
        _locationPath.empty() ? "" : _config.getLocation(_locationPath).getRoot()->getValue();
    if (tempRoot.empty()) {
        tempRoot = _config.getRoot()->getValue();
    }
    if (tempRoot.empty()) {
        tempRoot = DEFAULT_ROOT;
    }
    _root = tempRoot;
}

void HttpRequest::parseAllowMethods() {
    if (!_locationPath.empty()) {
        _allowMethods = _config.getLocation(_locationPath).getAllowMethods()->getValue();
        if (!_allowMethods.empty()) return;
    }
    _allowMethods = _config.getAllowMethods()->getValue();
}

void HttpRequest::parseAutoindex() {
    std::string tempAutoindex = "";
    if (!_locationPath.empty()) {
        tempAutoindex = _config.getLocation(_locationPath).getAutoindex()->getValue();
        if (!tempAutoindex.empty()) {
            _autoindex = tempAutoindex == "on";
            return;
        }
    }
    tempAutoindex = _config.getAutoindex()->getValue();
    _autoindex = tempAutoindex == "on";
}

void HttpRequest::parseLocation() {
    size_t firstSlash = _cleanUri.find_first_of('/');
    size_t secondSlash = _cleanUri.find_first_of('/', firstSlash + 1);
    if (secondSlash == std::string::npos) {
        _locationPath = "";
        return;
    }
    std::string tempLocationPath = _cleanUri.substr(0, secondSlash + 1);
    try {
        _config.getLocation(tempLocationPath);
        _locationPath = tempLocationPath;
        this->_cleanUri.erase(
            0, tempLocationPath.size());  // remove location to make the "alias behavior"
    } catch (const std::exception& e) {
        _locationPath = "";
    }
}

void HttpRequest::detectCgiAndMime() {
    std::string filePath = _root + _cleanUri + _index;
    size_t dotPos = filePath.rfind('.');
    if (dotPos != std::string::npos) {
        std::string ext = filePath.substr(dotPos);

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
        else if (ext == ".gif")
            _mimeType = "image/gif";
        else
            _mimeType = "text/html";

    } else {
        _isCgi = false;
        _cgiType = CGI_NONE;
        _mimeType = "text/html";
    }
}

void HttpRequest::parseRedirect() {
    std::string tempRedirect =
        _locationPath.empty() ? "" : _config.getLocation(_locationPath).getRedirect()->getValue();
    // if (tempRedirect.empty()) {
    //     tempRedirect = _config.getRedirect()->getValue();
    // }
    std::cout << "tempRedirect: " << tempRedirect << std::endl;
    std::cout << "locationPath: " << _locationPath << std::endl;
    _redirect = tempRedirect;
}

void HttpRequest::parseResponse() {
    std::string strMethod = methodToString(_method);

    if (std::find(_allowMethods.begin(), _allowMethods.end(), strMethod) == _allowMethods.end()) {
        this->buildErrorResponse(METHOD_NOT_ALLOWED);
        return;
    }

    std::string filePath = _root + _cleanUri + _index;
    std::string fileContent = Utils::readFile(filePath);
    if (fileContent.empty() && _method != METHOD_DELETE) {
        // TODO [CGI] - now it returns error, because we can't open HTML with that. we can fix later
        if (Utils::isDirectory(filePath) && this->_autoindex) {
            this->buildAutoindexResponse(filePath);
        } else {
            std::cerr << "Error: Could not open file: " << Utils::cleanSlashes(filePath)
                      << std::endl;
            this->buildErrorResponse(NOT_FOUND);
        }
        return;
    }

    // IT HAS TO BE BEFORE CGI because the path removed
    if (_method == METHOD_DELETE) {
        if (fileContent.empty()) {
            this->buildErrorResponse(NOT_FOUND_DELETE);
            return;
        }
        if (std::remove(filePath.c_str()) == 0) {
            this->buildErrorResponse(NO_CONTENT);
            return;
        } else
            this->buildErrorResponse(NOT_FOUND_DELETE);
    }

    if (_isCgi) {
        std::cout << "[CGI] Detected Python Script " << filePath << std::endl;

        CgiHandler cgi(filePath);
        std::cout << "[DEBUG] CGI Body: " << _body << std::endl;
        std::string cgiResult = cgi.execute(_body, _method == METHOD_POST ? "POST" : "GET");

        if (cgiResult == CGI_ERROR_RESPONSE) {
            std::cerr << "[CGI] Error occurred, sending 500 Internal Server Error\n";
            buildErrorResponse(INTERNAL_SERVER_ERROR);
            return;
        }

        // I wrap CGI response in HTML style now
        this->buildOKResponse(cgiResult, "text/html");
        if (_method == METHOD_POST) {
            std::ostringstream postFile;
            postFile << time(NULL);
            std::string filePath = _config.getRoot()->getValue() + postFile.str() + ".txt";
            std::ofstream outFile(filePath.c_str(), std::ios::out | std::ios::binary);
            if (!outFile) {
                buildErrorResponse(NOT_FOUND_DELETE);
                return;
            }
            outFile << _body;
            outFile.close();
        }
        return;
    }

    if (_method == METHOD_POST) {
        if (_body.empty()) {
            buildErrorResponse(BAD_REQUEST);
            return;
        }

        std::ostringstream postFile;
        postFile << time(NULL);
        std::string filePath = _config.getRoot()->getValue() + postFile.str() + ".txt";
        std::ofstream outFile(filePath.c_str(), std::ios::out | std::ios::binary);
        if (!outFile) {
            buildErrorResponse(NOT_FOUND_DELETE);
            return;
        }
        outFile << _body;
        outFile.close();

        std::string successHtml =
            "<html>\n<body>\n<h1>Upload successful</h1>\n<p>Saved to: " + filePath +
            "</p>\n</body>\n</html>\n";
        buildOKResponse(successHtml, "text/html");
        return;
    }
    this->buildOKResponse(fileContent, _mimeType);
}

void HttpRequest::initFromRaw() {
    if (!parseRequestLine()) {
        this->buildErrorResponse(BAD_REQUEST);
        return;
    }
    parseLocation();
    parseRedirect();
    if (!_redirect.empty()) {
        this->buildOKResponse("", "text/html");
        return;
    }
    parseAutoindex();
    parseRoot();
    parseIndex();
    parseHeaders();
    parseBody();
    detectCgiAndMime();
    parseAllowMethods();
    parseResponse();
}

HttpMethod HttpRequest::getMethod() const { return _method; };
std::string HttpRequest::getRawRequest() const { return _rawRequest; };
std::string HttpRequest::getCleanUri() const { return _cleanUri; };
std::map<std::string, std::string> HttpRequest::getQueryParams() const { return _queryParams; };
HttpVersion HttpRequest::getHttpVersion() const { return _httpVersion; };
std::map<std::string, std::string> HttpRequest::getHeaders() const { return _headers; };
std::string HttpRequest::getBody() const { return _body; };
std::string HttpRequest::getMimeType() const { return _mimeType; };
CgiType HttpRequest::getCgiType() const { return _cgiType; };
bool HttpRequest::getIsCgi() const { return _isCgi; };
std::string HttpRequest::getResponse() const { return _response; };