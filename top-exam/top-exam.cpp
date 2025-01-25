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


void menu(Server& server)
{
    int input;
    string prefix = "_quest_";
    string quest;
    while (true)
    {
        cout << RESET_COLOR << endl << "1 - Начать голосование\n2 - Закончить работу\n";
        cin >> input;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << WARNING_COLOR << "Неправильный ввод. Введите ЧИСЛО: " << RESET_COLOR << endl;;
            std::cin >> input;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (input)
        {
        case 1:
            cout << "Введите вопрос для голосования: ";  
            getline(cin, quest);
            cout << "==================================================================" << endl;
            server.setQuestion(quest);
            server.sendMessageToAll(prefix + quest);
            server.getResponse();
            break;
        case 2:
            exit(0);
            break;
        default:
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Server server;

    std::thread menuThread(menu, std::ref(server));

    server.start();

    if (menuThread.joinable()) {
        menuThread.join();
    }

    return 0;
}