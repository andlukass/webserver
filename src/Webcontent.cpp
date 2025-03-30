/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webcontent.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngtina1999 <ngtina1999@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 00:51:23 by ngtina1999        #+#    #+#             */
/*   Updated: 2025/03/31 01:20:28 by ngtina1999       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webcontent.hpp"

Webcontent::Webcontent() {
}

Webcontent::~Webcontent() {
}

Webcontent::Webcontent(const Webcontent &copy) {
	(void)copy;
}

Webcontent & Webcontent::operator=(const Webcontent &rhs) {
	(void)rhs;
	return(*this);
}

void	Webcontent::contentManager(int clientFd, std::string root) {
	// Read the HTML file

	//TODO:handle request
    // TINA: use getter
    std::ifstream file("./src/webcontent/webcontent.html", std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open HTML file" << std::endl;
        close(clientFd);
        return;
    }
    std::string htmlContent((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    // Build HTTP response header
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";

    // Convert size to string using stringstream (for C++98)
    std::stringstream ss;
    ss << htmlContent.size();
    std::string contentLength = ss.str();
    response += "Content-Length: " + contentLength + "\r\n";

    response += "Connection: close\r\n\r\n";  // Close connection after response
    response += htmlContent;

    // Send the response to the client
    send(clientFd, response.c_str(), response.size(), 0);

    std::cout << "HTML content sent to client!" << std::endl;

}