#include "Client.h"
#include <iostream>
#include <boost/asio.hpp>


Client::Client(boost::asio::ip::tcp::socket s) : socket(std::move(s)) {}

std::string Client::readFromClient() {
    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");

        std::istream stream(&buffer);
        std::string data;
        std::getline(stream, data);
        return data;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "";
    }
}

void Client::sendMessageToClient(const std::string& message) {
    try {
        boost::asio::write(socket, boost::asio::buffer(message + "\n"));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
