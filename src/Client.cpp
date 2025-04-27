#include "../includes/Client.hpp"

Client::Client(int fd) : _fd(fd) {}

Client::~Client() {}

void Client::receive() {
    ssize_t receivedDataLength = recv(this->_fd, this->_buffer, sizeof(this->_buffer) - 1, 0);
    if (receivedDataLength <= 0) {
        this->close();
    }
    std::cout << "====Received data (" << receivedDataLength << "): \n" << this->_buffer <<  "======================\n" << std::endl;
    this->_buffer[receivedDataLength] = '\0';
}

void Client::send(const std::string& response) {
    ssize_t sentDataLength = ::send(this->_fd, response.c_str(), response.size(), 0);
    std::cout << "Sent data: (" << sentDataLength << ") fd:("<< this->_fd << "): \n" << response <<  "======================\n" << std::endl;
    if (sentDataLength <= 0) {
        this->close();
    }
}

void Client::close() {
    ::close(this->_fd);
}

std::string Client::getBuffer() const {
    return std::string(this->_buffer);
}
