#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <nlohmann/json.hpp>
#include <codecvt>
#include <Windows.h>
#include <locale>
#include <cstdlib>

using json = nlohmann::json;
using boost::asio::ip::tcp;

using namespace std;

enum Ships
{
    empty = 0,
    ship = 1,
    dead = 2,
    miss = 3
};
void write_to_server(tcp::socket& socket) {
    try {
        cout << "Введите координату по X: ";
        int x;
        cin >> x;
        cout << "Введите координату по оси Y: ";
        int y;
        cin >> y;
        json j;
        j["x"] = x-1;
        j["y"] = y-1;
        string message = j.dump();
        boost::asio::write(socket, boost::asio::buffer(message));
    }
    catch (const exception& e) {
        cerr << "Write error: " << e.what() << "\n";
    }
}

void read_from_server(tcp::socket& socket) {
    try {
        char reply[1024];
        while (true) {
            size_t reply_length = socket.read_some(boost::asio::buffer(reply));
            json j;
            j = json::parse(string(reply, reply_length));
            if (j["flag"] == 2){ cout << j["message"]; return;}
            system("cls");
            cout << "Карта: " << endl;
            cout << "  ";
            for (int i = 0; i < 10; i++)
            {
                cout << i + 1 << " ";
            }
            cout << endl;
            for (int y = 0; y < 10; y++)
            {
                cout << y + 1 << " ";
                for (int x = 0; x < 10; x++)
                {   
                    switch (static_cast<Ships>(j["myMap"]["map"][y][x]))
                    {
                        case Ships::ship:
                            cout << "#" << " ";
                            break;
                        case Ships::empty:
                            cout << "-" << " ";
                            break;
                        case Ships::dead:
                            cout << "X" << " ";
                            break;
                        case Ships::miss:
                            cout << "*" << " ";
                            break;
                        default:break;
                    }
                }
                cout << endl;
            }
            cout << endl << "Карта соперника: " << endl;
            for (int y = 0; y < 10; y++)
            {
                cout << y + 1 << " ";
                for (int x = 0; x < 10; x++)
                {
                    switch (static_cast<Ships>(j["enemyMap"]["map"][y][x]))
                    {
                    case Ships::ship:
                        cout << "#" << " ";
                        break;
                    case Ships::empty:
                        cout << "-" << " ";
                        break;
                    case Ships::miss:
                        cout << "*" << " ";
                        break;
                    case Ships::dead:
                        cout << "X" << " ";
                        break;
                    default:break;
                    }
                }
                cout << endl;
            }
            if (j["flag"] == 1)
            {
                write_to_server(socket);
            }
            else if (j["flag"] == 2)
            if(j["message"] != "" ) cout << j["message"];
        }
    }
    catch (exception& e) {
        cerr << "Read error: " << e.what() << "\n";
    }
}



int main() {
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "60");

        // Подключение к серверу
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        read_from_server(socket);
        while (true);
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}