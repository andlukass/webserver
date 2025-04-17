# Future Steps for HTTP Server Development

## 1. **Receive HTTP Requests (Get Data)** - DONE
- Implement a `recv()` method to read HTTP request data.
  - Read the incoming HTTP request line (e.g., `GET /index.html HTTP/1.1`).
- Parse the HTTP headers and any body (e.g., for POST requests).

## 2. **Parse the HTTP Request** - DONE
- Parse the HTTP Method (e.g., `GET`, `POST`).
- Extract path (e.g., `/index.html`).
- Extract headers (key-value pairs like `Host`, `Content-Type`, etc.).
- Handle POST body (if applicable).

## 3. **Match Path with Location in Configuration**
- Match the request path to the corresponding `location` in the config.
    - Example: if the path is `/upload`, find the corresponding location block.
- Verify allowed methods for that location (e.g., GET/POST).

## 4. **Generate HTTP Response**
- Handle GET requests:
    - Read the file from the disk (e.g., `/index.html`).
    - Generate a response with the file contents and appropriate headers (like `Content-Type`).
- Handle POST requests:
    - Receive data (e.g., form submissions) and generate a response.
    - Handle `client_max_body_size` to reject requests that exceed the limit. - ONLY PART NOT DONE
- Handle error pages (e.g., 404 Not Found):
    - If the file is not found, return a custom error page based on the config.

## 5. **Send HTTP Response** - DONE
- Send the response headers (status, content type, etc.).
- Send the response body (file data for GET requests, or a success message for POST).

## 6. **Handle File Serving (Static Files)**
- Read the requested file from the disk.
    - Check the `root` directive in the config to find the base directory for files.
- Send the file to the client with proper headers.

## 7. **Handle POST Requests & File Uploads**
- Handle file uploads:
    - Parse the multipart data.
    - Save files to the configured upload directory.

## 8. **Implement Error Handling (404, 500, etc.)** - DONE
- Handle errors like 404 (File not found), 500 (Internal server error), etc.
- Serve custom error pages based on configuration (`error_page` directive).

## 9. **Config Validator**
- Implement a **configuration validator**:
    - Check for missing required parameters in the config (e.g., `server_name`, `root`).
    - Validate paths and ensure they exist (e.g., `root` and `error_page`).
    - Ensure `client_max_body_size` is a valid positive number.
    - Check for valid HTTP methods in `allow_methods`.

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

---

## **Summary of Next Steps**
1. **Receive HTTP requests** with `recv()`.
2. **Parse the HTTP request** (method, path, headers).
3. **Match path** to the correct `location` in the config.
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
for all the I/O operations between the client and the server (listen included), • poll() (or equivalent) must monitor both reading and writing simultaneously

Tina:
• Your server must have default error pages if none are provide, • You must be able to serve a fully static website, Clients must be able to upload files.
• You need at least the GET, POST, and DELETE methods, ◦ Enable or disable directory listing. autoindex

Yulia:
Lear CGI, implement some working part
◦ Execute CGI based on certain file extension (for example .php).


• Set the maximum allowed size for client request bodies.
