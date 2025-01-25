#include "Server.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <thread>

#define PORT 80
Server::Server() : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT)) {}

void Server::start() {
    std::thread serverThread(&Server::acceptConnections, this);  
    serverThread.detach(); 

    io_context.run();  
}

void Server::acceptConnections() {
    auto new_socket = new boost::asio::ip::tcp::socket(io_context);
    acceptor.async_accept(*new_socket, [this, new_socket](const boost::system::error_code& error) {
        if (!error) {
            Client client(std::move(*new_socket));
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(client);
                std::cout << "Подключение успешно!" << std::endl;
            }
            std::thread(&Server::handleClient, this, client).detach(); 
        }
        acceptConnections();  
        });
}

void Server::handleClient(Client& client) {
    try {
        while (true) {
            std::string message = client.readFromClient();
            if (!message.empty()) {
                std::cout << message << std::endl;
            }           
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Client disconnected: " << e.what() << std::endl;
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
        }
    }
}

void Server::sendToAllClients(const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& client : clients) {
        client.sendMessageToClient(message);
    }
}