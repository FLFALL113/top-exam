#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include "Libs.h"
#include <nlohmann/json.hpp>
#include "Client.h"
#include "Server.h"

#define PORT 60
#define WARNING_COLOR "\033[31m"
#define SUCCESSFULLY_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std;

void game(Server& server)
{
    
    while (server.countClient != 2)
    {
    }
    string field = "";
    field += "\n | | \n | | \n | | ";
    server.sendMessageToAll(field);
    int queue = 0;
    while (true)
    {
        server.sendMessage(server.clients_[queue], "_Ваш ход: ");
        string response = server.getResponse(server.clients_[queue]);
        queue = queue == 0 ? 1 : 0;
        cout << response;
    }
    
}
int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Server server;
    thread t(game,ref(server));
    server.start();
    while (true);

    return 0;
}