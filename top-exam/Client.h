#pragma once
#include <boost/asio.hpp>
#include <string>

class Client {
public:
    explicit Client(boost::asio::ip::tcp::socket s);
    std::string readFromClient();
    void sendMessageToClient(const std::string& message);
    bool operator==(const Client& other) const {
        // Ќапример, сравниваем по адресу сокета
        return socket.remote_endpoint() == other.socket.remote_endpoint();
    }

private:
    boost::asio::ip::tcp::socket socket;
};
