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
#include "Game.h"

#define PORT 60
#define WARNING_COLOR "\033[31m"
#define SUCCESSFULLY_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

using json = nlohmann::json;
using boost::asio::ip::tcp;
using namespace std;


int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Game game;
    Server server;
    server.start();
    while (true);

    return 0;
}