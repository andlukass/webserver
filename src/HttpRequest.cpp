#include "../includes/HttpRequest.hpp"

#include <sstream>

HttpRequest::HttpRequest(const std::string& uri)
    : _rawUri(uri),
      _method(METHOD_UNKNOWN),
      _httpVersion(HTTP_VERSION_UNKNOWN),
      _cgiType(CGI_NONE),
      _isCgi(false),
      _isValid(true) {}

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
    size_t firstLineEnd = _rawUri.find("\r\n");
    if (firstLineEnd == std::string::npos) return (_isValid = false);

    std::string startLine = _rawUri.substr(0, firstLineEnd);

    std::istringstream iss(startLine);
    std::string methodStr, uriStr, versionStr;
    iss >> methodStr >> uriStr >> versionStr;

    if (methodStr.empty() || uriStr.empty() || versionStr.empty()) {
        _isValid = false;
        return false;
    }

    _method = parseMethod(methodStr);
    _cleanUri = uriStr;
    _httpVersion = parseHttpVersion(versionStr);

    if (_method == METHOD_UNKNOWN || _httpVersion == HTTP_VERSION_UNKNOWN) {
        _isValid = false;
        return false;
    }
    return true;
}

void HttpRequest::parseHeaders(const std::string& headersBlock) {
    std::istringstream stream(headersBlock);
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
    size_t bodyStart = _rawUri.find("\r\n\r\n");
    // checking if body is present. we don't validate if it's supposed to be at the moment
    if (bodyStart != std::string::npos) {
        // +4 here to skip empty line in between headers and body
        bodyStart += 4;
        if (bodyStart < _rawUri.size()) _body = _rawUri.substr(bodyStart);
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
            _mimeType = "text/plain";

    } else {
        _isCgi = false;
        _cgiType = CGI_NONE;
        _mimeType = "text/plain";
    }
    return;
}

void HttpRequest::initFromRaw() {
    if (!parseRequestLine()) return;

    size_t firstLineEnd = _rawUri.find("\r\n");
    if (firstLineEnd == std::string::npos) {
        _isValid = false;
        return;
    }
    size_t headersStart = firstLineEnd + 2;

    size_t headersEnd = _rawUri.find("\r\n\r\n");
    if (headersEnd == std::string::npos) {
        _isValid = false;
        return;
    }

    parseHeaders(_rawUri.substr(headersStart, headersEnd));
    parseBody();
    detectCgiAndMime();
}