#include "../includes/Client.hpp"

Client::Client(int fd, const ServerDirective& serverConfig) : _fd(fd), _serverConfig(serverConfig) {}

Client::~Client() {}

ssize_t Client::receive() {
    ssize_t receivedDataLength = recv(this->_fd, this->_buffer, sizeof(this->_buffer) - 1, 0);
    this->_buffer[receivedDataLength] = '\0';
    std::cout << "\n====Received data (" << receivedDataLength << "): \n" << this->_buffer <<  "======================\n" << std::endl;
    return receivedDataLength;
}

ssize_t Client::send(const std::string& response) {
    ssize_t sentDataLength = ::send(this->_fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    std::cout << "Enviado com sucesso: " << sentDataLength << " bytes" << std::endl;
    return sentDataLength;
}

void Client::close() {
    ::close(this->_fd);
}

std::string Client::getBuffer() const { return std::string(this->_buffer); }
const ServerDirective& Client::getConfig() const { return this->_serverConfig; }
int Client::getFd() const { return this->_fd; }
