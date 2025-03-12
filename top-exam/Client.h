#pragma once
#include "Libs.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Client
{
public:
    Client(tcp::socket s);
    string acceptResponse();
    string getName() const;

    tcp::socket& getSocket();
    string getResponse();
    json getJson();

    string question;
private:
    string name;
    void saveResponse();
    boost::asio::ip::tcp::socket socket_;
    json json_;
    string response_;
    
};