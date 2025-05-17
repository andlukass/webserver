#include "../includes/Client.hpp"

Client::Client(int fd, const ServerDirective& serverConfig) : _fd(fd), _serverConfig(serverConfig) {}

Client::~Client() {}

ssize_t Client::receive() {
    char tempBuffer[2000000];
    ssize_t totalReceived = 0;
    this->_buffer.clear();
    while (true) {
        ssize_t receivedDataLength = recv(this->_fd, tempBuffer, sizeof(tempBuffer) - 1, MSG_DONTWAIT);
        if (receivedDataLength <= 0)
            break;
        tempBuffer[receivedDataLength] = '\0';
        this->_buffer.append(tempBuffer, receivedDataLength);
        totalReceived += receivedDataLength;
        if (receivedDataLength < (ssize_t)(sizeof(tempBuffer) - 1))
            break;
    }
    return this->_buffer.size();
}

ssize_t Client::send(const std::string& response) {
    ssize_t sentDataLength = ::send(this->_fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    std::cout << "Enviado com sucesso: " << sentDataLength << " bytes" << std::endl;
    return sentDataLength;
}

void Client::close() {
    ::close(this->_fd);
}

std::string Client::getBuffer() const { return this->_buffer; }
const ServerDirective& Client::getConfig() const { return this->_serverConfig; }
int Client::getFd() const { return this->_fd; }
