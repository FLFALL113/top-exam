#pragma once
#include "Libs.h"
#include "Client.h"
#include "FileSystem.h"
#include "ListFiles.h"

class Server {
private:
    boost::asio::io_context io_context_;
    std::vector<Client> clients_;
    std::mutex client_mutex_;
    void accept_connections(tcp::acceptor& acceptor);
    string question;
public:
    Server() {};
    void start();
    void sendMessageToAll(const std::string& msg);
    void getResponse();
    void setQuestion(const std::string& quest);
};