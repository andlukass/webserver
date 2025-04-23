#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest(const std::string& uri)
    : _rawUri(uri),
      _method(METHOD_UNKNOWN),
      _httpVersion(HTTP_VERSION_UNKNOWN),
      _cgiType(CGI_NONE),
      _isCgi(false),
      _isValid(true) {}

HttpRequest::~HttpRequest() {}

bool HttpRequest::parseRequestLine() {
    size_t lineEnd = _rawUri.find("\r\n");
    if (lineEnd == std::string::npos) return (_isValid = false);
}

void parseHeaders(const std::string& headersBlock) {}
void parseBody(const std::string& rawBody) {}
void detectCgiAndMime() {}

void HttpRequest::initFromRaw() {
    if (!parseRequestLine()) return;
    // 1. Разбить _rawUri на части: первая строка, заголовки, тело
    // 2. Вызвать parseRequestLine()
    // 3. Вызвать parseHeaders()
    // 4. Вызвать parseBody()
    // 5. Вызвать detectCgiAndMime()
}