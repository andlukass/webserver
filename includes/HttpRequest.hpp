#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include "./directives/ServerDirective.hpp"
#include "../includes/cgi/CgiHandler.hpp"

#define DEFAULT_ROOT "./"
#define DEFAULT_INDEX "/index.html"

enum HttpMethod { METHOD_UNKNOWN, METHOD_GET, METHOD_POST, METHOD_DELETE };
enum CgiType { CGI_NONE, CGI_PYTHON };
enum HttpVersion { HTTP_VERSION_UNKNOWN, HTTP_1_0, HTTP_1_1 };

class HttpRequest {
   private:
    HttpMethod _method;
    std::string _rawRequest;
    std::string _cleanUri;
    std::string _rawHeaders;
    std::map<std::string, std::string> _queryParams;
    HttpVersion _httpVersion;
    std::map<std::string, std::string> _headers;
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

    // cookies;
    // contentType;
    // contentLength;

    bool parseRequestLine();
    void parseHeaders();
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
   public:
    HttpRequest(const ServerDirective& config, const std::string& request);
    ~HttpRequest();

    // getters
    HttpMethod getMethod() const;
    std::string getRawRequest() const;
    std::string getCleanUri() const;
    std::string getPath() const;
    std::map<std::string, std::string> getQueryParams() const;
    HttpVersion getHttpVersion() const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getBody() const;
    std::string getMimeType() const;
    CgiType getCgiType() const;
    bool getIsCgi() const;
    std::string getResponse() const;

	//void	HttpRequest::handlePOSTRequest();TODO
};