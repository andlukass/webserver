#include "../includes/Client.hpp"

Client::Client(int fd, const ServerDirective& serverConfig)
    : _fd(fd), _serverConfig(serverConfig) {}

Client::~Client() {}

/// Receive data from client socket
/// - Reads into temp buffer using non-blocking recv()
/// - Appends to internal _buffer
/// - Stops on error or when less than buffer size received
ssize_t Client::receive() {
    char tempBuffer[8192];  // Large temporary buffer
    this->_buffer.clear();     // Clear previous buffer
    while (true) {
        // MSG_DONTWAIT ensures non-blocking mode of recv even if there is no data yet
        // if there is no data, recv() will return -1 and errno will be EWOULDBLOCK or EAGAIN
        ssize_t receivedDataLength =
            recv(this->_fd, tempBuffer, sizeof(tempBuffer) - 1, MSG_DONTWAIT);
        // If recv returns 0 (connection closed) or -1 (error), stop reading
        if (receivedDataLength <= 0) break;
        // Null-terminate the buffer
        tempBuffer[receivedDataLength] = '\0';
        // Append received data to internal buffer
        this->_buffer.append(tempBuffer, receivedDataLength);
        // If less than full buffer was read — assume we're done for now
        if (receivedDataLength < (ssize_t)(sizeof(tempBuffer) - 1)) break;
    }
    // Return total number of bytes stored in internal buffer
    return this->_buffer.size();
}

/// Send response to client socket
/// - Uses MSG_NOSIGNAL to avoid SIGPIPE on broken connection (if something goes wrong, send() will
/// return -1, won't break program)
/// - Returns number of bytes actually sent
ssize_t Client::send(const std::string& response) {
    ssize_t sentDataLength = ::send(this->_fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    if (sentDataLength < 0) {
        std::cerr << "Error sending data to client" << std::endl;
    } else if (sentDataLength == 0) {
        std::cerr << "Warning: send() returned 0, no data was sent." << std::endl;
    } else {
        std::cout << "Successfully sent: " << sentDataLength << " bytes" << std::endl;
    }

    return sentDataLength;
}

void Client::close() { ::close(this->_fd); }

std::string Client::getBuffer() const { return this->_buffer; }
const ServerDirective& Client::getConfig() const { return this->_serverConfig; }
int Client::getFd() const { return this->_fd; }
