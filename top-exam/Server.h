#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Libs.h"
#include "Client.h"
#include "FileSystem.h"
#include "ListFiles.h"
#include "Game.h"
#include <chrono>
#include <ctime>

class Server {
private:
    boost::asio::io_context io_context_;
    
    std::mutex client_mutex_;
    void accept_connections(tcp::acceptor& acceptor);
public:
    std::vector<Client> clients_;
    int countClient = 0;
    Server() {};
    void start();
    void sendMessageToAll(const std::string& msg);
    json getResponse(Client & client);
    void sendMessage(Client& client, const string& msg);
    void game();
};