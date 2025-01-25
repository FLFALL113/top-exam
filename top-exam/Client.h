#pragma once
#include "Libs.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Client
{
public:
    Client(tcp::socket s);
    void acceptResponse();


    tcp::socket& getSocket();
    string getResponse();
    json getJson();

    string question;
private:
    void saveResponse();
    boost::asio::ip::tcp::socket socket_;
    json json_;
    string response_;
    
};