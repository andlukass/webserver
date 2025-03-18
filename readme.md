# DOCS

## config file:

There is only one top directive: "server". There can be multiple servers directives, each one nested with their own directives:

### client_max_body_size

    The maximum file size which the server will accept in each req.
    Can be a positive integer from 0 to 2147483647 (if 0, all restrictions are disabled).
    If more than one is declared inside the same server, the last one will be considered.

### listen

    The ip and port that the server will be listening.
    Can be one of the following:
    - a string containing only numbers, followed by a colon, and a port number
    - an IPV4 address
    - an IPV4 address followed by a colon, and a port number
    - just a port number
    If more than one is declared inside the same server, the last one will be considered.

### server_name

    A list of strings representing the names of the server.
    If more than one is declared inside the same server, the last one will be considered.

#### root

    The original path inside the server's file system from which the location path will be redirected.

#### index

    The file list to the to search inside the server's file system that it will redirect to, in case the requested URL is a directory.

### error_page

    The list of status error pages and which file it should be redirected.
    Consists of multiple strings separated by white spaces.
    The first are positive integers between 300 and 599 representing the error status codes.
    And the last string is the file to which those errors will be redirected.
    Can have multiple error_page declarations inside the same server, in the case of the same value, the last one will be considered.

### location

    The first argument is the url path that you want to be configured.
    The following argument, is his own nested directives, that are the following:

---

### root && index

    Within a location block, the root and index directives can also be defined. If specified, they will override the corresponding directives set in the server block for that specific location.

#### autoindex

    Indicates whether the server should show the directory listing when the requested URL is a directory and it doesn't have an index set.
    It can be a string containing 'on' or 'off'.

#### allow_methods

    The list of allowed HTTP methods inside the location.
    Can be one or multiple of the following string separeted by white spaces:
    GET | HEAD | POST | PUT | DELETE | PATCH | TRACE | CONNECT | OPTIONS

---
