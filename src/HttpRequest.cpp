#include "../includes/HttpRequest.hpp"

#include <unistd.h>

#include <cerrno>

enum ErrorStatus {
    NOT_FOUND = 404,
    FORBIDDEN = 403,
    NOT_FOUND_DELETE = 600,
    METHOD_NOT_ALLOWED = 405,
    BAD_REQUEST = 400,
    PAYLOAD_TOO_LARGE = 413,
    NO_CONTENT = 204,
    INTERNAL_SERVER_ERROR = 500,
    OK = 200,
    FOUND = 302
};

std::string statusToString(int errorStatus) {
    switch (errorStatus) {
        case NOT_FOUND:
            return "404 Not Found";
        case FORBIDDEN:
            return "403 Forbidden";
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
        case OK:
            return "200 OK";
        case FOUND:
            return "302 Found";
        default:
            return "NOT HANDLED ERROR";
    }
}

std::string methodToString(HttpMethod method) {
    switch (method) {
        case METHOD_GET:
            return "GET";
        case METHOD_POST:
            return "POST";
        case METHOD_DELETE:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}

void HttpRequest::logResponse(int statusCode) const {
    std::cout << "[" << methodToString(_method) << "] " << _cleanUri << " → "
              << statusToString(statusCode);
    std::cout << std::endl;
}

void HttpRequest::buildErrorResponse(int errorStatus) {
    std::stringstream response;
    parseErrorPagePath(errorStatus);
    std::string fileContent = Utils::readFile(this->_errorPagePath);
    response << "HTTP/1.1 " << statusToString(errorStatus) << "\r\n";

    if (errorStatus == NO_CONTENT) {
        response << "Connection: close\r\n\r\n";
        this->_response = response.str();
        logResponse(errorStatus);
        return;
    }

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
    logResponse(errorStatus);
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
        std::cout << "Redirect: " << _redirect << std::endl;
        response << "HTTP/1.1 302 Found\r\n";
        response << "Location: " << _redirect << "\r\n";
        response << "Connection: close\r\n\r\n";
        this->_response = response.str();
        logResponse(FOUND);
        // std::cout << "response: " << this->_response << std::endl;
        return;
    }
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << fileContent.size() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << fileContent;
    logResponse(OK);
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

HttpRequest::HttpRequest(const ServerDirective& config, const std::string& request)
    : _method(METHOD_UNKNOWN),
      _rawRequest(request),
      _httpVersion(HTTP_VERSION_UNKNOWN),
      _isChunked(false),
      _cgiType(CGI_NONE),
      _isCgi(false),
      _config(config),
      _isValid(true),
      _errorCode(0) {
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

    // Strip query string (if present)
    size_t queryPos = _cleanUri.find('?');
    if (queryPos != std::string::npos) {
        _cleanUri = _cleanUri.substr(0, queryPos);
    }

    _httpVersion = parseHttpVersion(versionStr);
    _rawHeaders = _rawRequest.substr(headersStart, headersEnd);

    if (_method == METHOD_UNKNOWN || _httpVersion == HTTP_VERSION_UNKNOWN) {
        return false;
    }
    return true;
}

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool HttpRequest::parseHeaders() {
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

        if (key == "Host") {
            // missing Host header isn't allowed
            if (value.empty()) {
                buildErrorResponse(BAD_REQUEST);
                return false;
            }

            // split host:port
            size_t colonPos = value.find(':');
            std::string hostOnly =
                (colonPos != std::string::npos) ? value.substr(0, colonPos) : value;

            std::vector<std::string> serverNames = _config.getServerName()->getValue();
            // std::cout << "THIS IS THE VALUE: " << hostOnly << std::endl;
            //std::cout << "THIS IS THE IP" <<  _config.getListen()->getIp() << std::endl;
            for (size_t i = 0; i < serverNames.size(); i++)
                // std::cout << "THIS IS THE SERVER NAME: " << serverNames[i] << std::endl;
                if ((std::find(serverNames.begin(), serverNames.end(), hostOnly) ==
                    serverNames.end()) && hostOnly != _config.getListen()->getIp()) {
                    buildErrorResponse(FORBIDDEN);
                    return false;
                }
        }

        if (key == "Transfer-Encoding" && toLower(value) == "chunked") {
            this->_isChunked = true;
        }
    }
    return true;
}

std::string unchunkBody(const std::string& rawBody) {
    std::istringstream stream(rawBody);
    std::string result;
    std::string line;

    while (std::getline(stream, line)) {
        if (!line.empty() && line[line.length() - 1] == '\r') line.erase(line.length() - 1);

        std::istringstream hexStream(line);
        size_t chunkSize;
        hexStream >> std::hex >> chunkSize;

        if (hexStream.fail()) {
            std::cerr << "[Unchunk] Failed to parse chunk size\n";
            break;
        }

        if (chunkSize == 0) break;

        char* buffer = new char[chunkSize];
        stream.read(buffer, chunkSize);
        result.append(buffer, chunkSize);
        delete[] buffer;

        stream.get();  // \r
        stream.get();  // \n
    }

    return result;
}

void HttpRequest::parseBody() {
    size_t bodyStart = _rawRequest.find("\r\n\r\n");

    // checking if body is present. we don't validate if it's supposed to be at the moment
    if (bodyStart == std::string::npos) return;

    // +4 here to skip empty line in between headers and body
    bodyStart += 4;
    if (bodyStart >= _rawRequest.size()) return;
    std::string rawBody = _rawRequest.substr(bodyStart);

    if (_isChunked) {
        _body = unchunkBody(rawBody);
    } else {
        _body = rawBody;
    }
    if (_body.size() > _config.getClientMaxBodySize()->getValueBytes()) {
        _isValid = false;
        _errorCode = PAYLOAD_TOO_LARGE;
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

    // debug checks for root
    // if (tempRoot[0] != '/') {
    //     // relative path
    //     std::cout << "[parseRoot] Relative root: " << tempRoot << std::endl;
    //     // absolute path
    // } else {
    //     std::cout << "[parseRoot] Absolute root: " << tempRoot << std::endl;
    // }

    // checking that dir is accessible
    if (access(tempRoot.c_str(), F_OK) != 0) {
        std::cerr << "[parseRoot] Root path does not exist: " << tempRoot << std::endl;
        _isValid = false;
        _errorCode = NOT_FOUND;
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
        this->_cleanUri.erase(0, tempLocationPath.size());
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
        } else if (ext == ".sh") {
            _isCgi = true;
            _cgiType = CGI_BASH;
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
    if (!_locationPath.empty()) {
        std::string tempRedirect = _config.getLocation(_locationPath).getRedirect()->getValue();
        _redirect = tempRedirect;
    } else {
        std::string tempRedirect = _config.getRedirect()->getValue();
        _redirect = tempRedirect;
    }
}

bool HttpRequest::parseMultipartBody() {
    std::string contentType = _headers["Content-Type"];
    size_t boundaryPos = contentType.find("boundary=");

    if (boundaryPos == std::string::npos) {
        return false;
    }

    std::string boundary = "--" + contentType.substr(boundaryPos + 9);
    size_t pos = _body.find(boundary);
    while (pos != std::string::npos) {
        size_t next = _body.find(boundary, pos + boundary.length());
        std::string part = _body.substr(pos + boundary.length() + 2,
                                        next - pos - boundary.length() - 4);  // skip \r\n

        // parse headers from the part
        size_t headerEnd = part.find("\r\n\r\n");
        if (headerEnd == std::string::npos) break;  // incomplete multipart body

        std::string headers = part.substr(0, headerEnd);
        std::string content = part.substr(headerEnd + 4);  // skip header end

        if (headers.find("filename=") != std::string::npos) {
            // extract filename
            size_t fnStart = headers.find("filename=\"") + 10;
            size_t fnEnd = headers.find("\"", fnStart);
            std::string filename = headers.substr(fnStart, fnEnd - fnStart);

            // save file
            std::string filePath = _config.getRoot()->getValue() + filename;
            std::ofstream file(filePath.c_str(), std::ios::binary);
            file.write(content.c_str(), content.size());
            file.close();
        }
        pos = next;
    }
    return true;
}

void HttpRequest::parseResponse() {
    std::string strMethod = methodToString(_method);

    if (std::find(_allowMethods.begin(), _allowMethods.end(), strMethod) == _allowMethods.end()) {
        this->buildErrorResponse(METHOD_NOT_ALLOWED);
        return;
    }

    std::string filePath = _root + _cleanUri + _index;
    // Verify that we are not trying to delete directory
    if (_method == METHOD_DELETE) {
        if (filePath[filePath.size() - 1] == '/')
        {
            this->buildErrorResponse(NOT_FOUND_DELETE);
            return;
        }
    }
    std::string fileContent = Utils::readFile(filePath);

	// check if file exists
	std::ifstream fileStream(filePath.c_str());
	bool fileExists = fileStream.is_open();
	fileStream.close();
	if (!fileExists && _method != METHOD_DELETE)
	{
        if (Utils::isDirectory(filePath) && this->_autoindex) {
            this->buildAutoindexResponse(filePath);
        } else {
            std::cerr << "Error: Could not open file: " << Utils::cleanSlashes(filePath)
                      << std::endl;
            this->buildErrorResponse(NOT_FOUND);
        }
        return;
    }

    if (_method == METHOD_DELETE) {
        if (filePath[filePath.size() - 1] == '/')
        {
            this->buildErrorResponse(NOT_FOUND_DELETE);
            return;
        }
		if (!fileExists) {
            this->buildErrorResponse(NOT_FOUND_DELETE);
            return;
        }
        if (std::remove(filePath.c_str()) == 0) {
            this->buildErrorResponse(NO_CONTENT);
            return;
        } else
            this->buildErrorResponse(NOT_FOUND_DELETE);
        return;
    }

    if (_isCgi) {
        std::cout << "[CGI] Detected CGI Script " << filePath << std::endl;

        CgiHandler cgi(filePath);
        std::string cgiResult =
            cgi.execute(_body, _method == METHOD_POST ? "POST" : "GET", _cgiType);

        if (cgiResult == CGI_ERROR_RESPONSE) {
            std::cerr << "[CGI] Error occurred, sending 500 Internal Server Error\n";
            buildErrorResponse(INTERNAL_SERVER_ERROR);
            return;
        }

        if (cgiResult.empty()) {
            std::cerr << "[CGI ERROR] CGI returned empty response\n";
            buildErrorResponse(INTERNAL_SERVER_ERROR);
            return;
        }

        // CGI response in HTML style
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
        // multipart/form-data uploads
        if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos) {
            if (!parseMultipartBody()) {
                buildErrorResponse(BAD_REQUEST);
                return;
            }
            // send OK HTML response with redirect
            _redirect = "/upload-success";
            buildOKResponse("", "text/html");
            return;
        }
        // plain POST body uploads
        else {
            if (_body.empty()) {
                buildErrorResponse(BAD_REQUEST);
                return;
            }

            // create a filename with current timestamp
            std::ostringstream postFile;
            postFile << time(NULL);
            std::string filePath = _config.getRoot()->getValue() + postFile.str() + ".txt";

            // save POST body to the file
            std::ofstream outFile(filePath.c_str(), std::ios::out | std::ios::binary);
            if (!outFile) {
                buildErrorResponse(NOT_FOUND_DELETE);
                return;
            }
            outFile << _body;
            outFile.close();

            // send success HTML response
            std::string successHtml =
                "<html>\n<body>\n<h1>Upload successful</h1>\n<p>Saved to: " + filePath +
                "</p>\n</body>\n</html>\n";
            _redirect = "/cgi/submit-feedback.py";
            buildOKResponse(successHtml, "text/html");
            return;
        }
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
    if (!parseHeaders()) return;
    parseBody();
    // parseBody checks max_body_size, so we check if the flag was False, other methods don't use it
    if (!_isValid) {
        buildErrorResponse(_errorCode);
        return;
    }
    detectCgiAndMime();
    parseAllowMethods();
    parseResponse();
}

HttpMethod HttpRequest::getMethod() const { return _method; };
std::string HttpRequest::getRawRequest() const { return _rawRequest; };
std::string HttpRequest::getCleanUri() const { return _cleanUri; };
HttpVersion HttpRequest::getHttpVersion() const { return _httpVersion; };
std::map<std::string, std::string> HttpRequest::getHeaders() const { return _headers; };
std::string HttpRequest::getBody() const { return _body; };
std::string HttpRequest::getMimeType() const { return _mimeType; };
CgiType HttpRequest::getCgiType() const { return _cgiType; };
bool HttpRequest::getIsCgi() const { return _isCgi; };
std::string HttpRequest::getResponse() const { return _response; };