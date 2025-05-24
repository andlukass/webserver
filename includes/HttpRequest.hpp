#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <dirent.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include "Constants.hpp"
#include "cgi/CgiHandler.hpp"
#include "directives/ServerDirective.hpp"

#define DEFAULT_ROOT "./"
#define DEFAULT_INDEX "/index.html"

class HttpRequest {
   private:
    HttpMethod _method;
    std::string _rawRequest;
    std::string _cleanUri;
    std::string _rawHeaders;
    HttpVersion _httpVersion;
    std::map<std::string, std::string> _headers;
    bool _isChunked;
    std::string _body;
    std::string _mimeType;
    std::string _redirect;
    CgiType _cgiType;
    bool _isCgi;
    std::string _response;
    const ServerDirective& _config;

    std::string _locationPath;
    std::string _errorPagePath;
    bool _autoindex;
    std::vector<std::string> _allowMethods;
    std::string _index;
    std::string _root;
    std::string _host;

    bool _isValid;
    int _errorCode;

    // cookies;
    // contentType;
    // contentLength;

    bool parseRequestLine();
    bool parseHeaders();
    void parseRedirect();
    void parseBody();
    void detectCgiAndMime();
    void parseLocation();
    void parseResponse();
    void buildErrorResponse(int errorStatus);
    void parseErrorPagePath(int errorStatus);
    void buildAutoindexResponse(std::string filePath);
    void buildOKResponse(std::string fileContent, std::string contentType);
    void parseAllowMethods();
    void parseAutoindex();
    void parseIndex();
    void parseRoot();
    void initFromRaw();

    void logResponse(int statusCode) const;

    // std::string unchunkBody(const std::string& rawBody);

   public:
    HttpRequest(const ServerDirective& config, const std::string& request);
    ~HttpRequest();

    // getters
    HttpMethod getMethod() const;
    std::string getRawRequest() const;
    std::string getCleanUri() const;
    std::string getPath() const;
    HttpVersion getHttpVersion() const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getBody() const;
    // bool getIsChunked() const;
    std::string getMimeType() const;
    CgiType getCgiType() const;
    bool getIsCgi() const;
    std::string getResponse() const;
    // bool getIsValid() const;
    // bool getErrorCode() const;

    bool parseMultipartBody();
};

#endif