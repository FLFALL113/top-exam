#pragma once
#include "Client.h"
#include <boost/asio.hpp>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>

class Server {
public:
    Server();
    void start();
    void sendToAllClients(const std::string& message);

private:
    void acceptConnections();
    void handleClient(Client& client);

    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::vector<Client> clients;
    std::mutex clients_mutex;
};
