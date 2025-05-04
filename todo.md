# Future Steps for HTTP Server Development

## 7. **Handle POST Requests & File Uploads**
- Handle file uploads:
    - Parse the multipart data.
    - Save files to the configured upload directory.

## 10. **Check with Valgrind**
- Regularly run **Valgrind** to check for memory leaks and errors:
    - Verify there are no memory leaks when handling multiple requests.
    - Ensure there are no **access violations** or **uninitialized memory** issues.
    - Continuously test after each major change (e.g., after adding new features or fixing bugs).

## 11. **Optimize and Refactor**
- Refactor and optimize the code for efficiency and readability.
    - Handle edge cases (e.g., invalid headers, large requests).
    - Make the code more modular for easier maintenance and future features.
    - Improve error handling and logging for easier debugging and monitoring.
 
## 12. **Handle properly errors in the parser**
- When the parsers throws an error, we should properly send the user a message in the cli and make sure that there is no memory leaks.

## 13. **Handle client_max_body_size**
- It should really just send a 413 when the body size is bigger than the specified.

## 14. **Handle directory listing**
- "Already handled". Just need to build the html page to it (there is a TODO on HttpRequest.cpp). It should list all the files in the directory, like a "ls", each item should be clickable to redirect to the path of it (its only use the a href of html).

## 15. **Define an HTTP redirect**
- Idk exactly, those are the exact words of the subject.

## 16. **Handle server_name**
- Idk exactly, we need to see how it works.

## 1. **Receive HTTP Requests (Get Data)** - DONE
- Implement a `recv()` method to read HTTP request data.
  - Read the incoming HTTP request line (e.g., `GET /index.html HTTP/1.1`).
- Parse the HTTP headers and any body (e.g., for POST requests).

## 2. **Parse the HTTP Request** - DONE
- Parse the HTTP Method (e.g., `GET`, `POST`).
- Extract path (e.g., `/index.html`).
- Extract headers (key-value pairs like `Host`, `Content-Type`, etc.).
- Handle POST body (if applicable).

## 4. **Generate HTTP base on Location Response** - DONE
- Match the request path to the corresponding `location` in the config.
- Example: if the path is `/upload`, find the corresponding location block.
- Handle GET requests:
    - Here we should check first the config in the location block, then go to the server block. (index, root, etc.).
    - Read the file from the disk (e.g., `/index.html`).
    - Generate a response with the file contents and appropriate headers (like `Content-Type`).
- Handle POST requests:
    - Receive data (e.g., form submissions) and generate a response.
    - Handle `error_page` in error case.
- Handle error pages (e.g., 404 Not Found):
    - If the file is not found, return a custom error page based on the config.
    - Verify allowed methods for that location (e.g., GET/POST).

## 5. **Send HTTP Response** - DONE
- Send the response headers (status, content type, etc.).
- Send the response body (file data for GET requests, or a success message for POST).

## 8. **Implement Error Handling (404, 500, etc.)** - DONE
- Handle errors like 404 (File not found), 500 (Internal server error), etc.
- Serve custom error pages based on configuration (`error_page` directive).

## 9. **Config Validator** - DONE
- Implement a **configuration validator**:
    - Check for missing required parameters in the config (e.g., `server_name`, `root`).
    - Validate paths and ensure they exist (e.g., `root` and `error_page`).
    - Ensure `client_max_body_size` is a valid positive number.
    - Check for valid HTTP methods in `allow_methods`.
---

## **Summary of Next Steps**
1. **Receive HTTP requests** with `recv()`.
2. **Parse the HTTP request** (method, path, headers).
4. **Generate HTTP response** (handle GET/POST, static files, error pages).
5. **Send HTTP response** with proper headers and body.
6. **Serve static files** for GET requests.
7. **Handle POST requests** and file uploads.
8. **Implement error handling** (404, 500).
9. **Validate the config** for required fields, paths, and sizes.
10. **Check with Valgrind** to ensure there are no memory leaks or errors.
11. **Optimize and refactor** for performance and maintainability.

## Task split:
Lucas:
(OK) for all the I/O operations between the client and the server (listen included), • poll() (or equivalent) must monitor both reading and writing simultaneously

Tina:
• Your server must have default error pages if none are provide, • You must be able to serve a fully static website, Clients must be able to upload files.
• You need at least the GET, POST, and DELETE methods, ◦ Enable or disable directory listing. autoindex

Yulia:
Lear CGI, implement some working part
◦ Execute CGI based on certain file extension (for example .php).


• Set the maximum allowed size for client request bodies.


Lucas: maybe we can join the ServerConfig with Server in the future, it will be more clean and understandable

/*
Lucas: (is was in servermanager, since its all done, im letting it here)
TODO: Future Configuration Validations (maybe parser already validates some or all)

1. Unique IP:Port Check
Ensure no two servers are configured with the same IP:Port.
(OK. in the case of same ip and port, we should check if the server_name is different,
if it is not, nginx just send the request to the first server in the config file that matches the ip and port)

2. Required Configuration Fields
Verify required fields like server_name, root, and listen are present and valid.
(OK. server_name is not required, root is not required, but if not present, we should send a 500 error,
listen is verified if is valid at parser and throw an error if not, if not present, it will be 0.0.0.0:80(like nginx default))

3. client_max_body_size Validation
Ensure client_max_body_size is positive and reasonable.
(OK. checked in ClientMaxBodySize)

4. Check File Paths
Validate the existence of directories specified by root and other path-based fields.
(OK. checked in HttpRequest)

5. Method Restrictions (allow_methods)
Ensure allow_methods contains valid HTTP methods like GET, POST, etc.
(OK. checked in HttpRequest && AllowMethodsDirective)

6. Error Pages
Ensure paths for custom error pages (e.g., error_page 404) are valid.
(OK. checked in HttpRequest)
*/
