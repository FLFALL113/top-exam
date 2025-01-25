#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include "Libs.h"
#include <nlohmann/json.hpp>

#define WARNING_COLOR "\033[31m"
#define SUCCESSFULLY_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

using json = nlohmann::json;
using boost::asio::ip::tcp;

class Client
{
public:
    Client(tcp::socket s) : socket_(std::move(s)) { boost::asio::write(this->socket_, boost::asio::buffer("Ожидайте вопроса.")); }
    void acceptResponse()
    {
        try {
            boost::asio::streambuf buf;

            boost::asio::read_until(this->socket_, buf, '\n');

            std::istream input_stream(&buf);
            std::string response;
            std::getline(input_stream, response);

            if (response.empty()) {
                std::cerr << WARNING_COLOR "Получены пустые данные!" << RESET_COLOR << std::endl;
                return;  
            }

            std::cout << "Получен ответ: " << response << std::endl;

            json received_json = json::parse(response);
            json_ = received_json;
            std::cout << "Человек с именем: " << received_json["name"] << " ответил: " << std::endl;
            std::cout << received_json["message"] << std::endl;

            saveResponse();
        }
        catch (const boost::system::system_error& e) {
            std::cerr << WARNING_COLOR << "Ошибка при чтении ответа от клиента: " << e.what() << RESET_COLOR << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << WARNING_COLOR << "Общая ошибка при обработке ответа от клиента: " << e.what() << RESET_COLOR << std::endl;
        }
    }

    
    tcp::socket& getSocket() { return socket_; }
    string getResponse() { return response_; }
private:
    void saveResponse()
    {
        FileSystem fs;
        FileLister fl;
        string path = "responses/";
        vector<string> files = fl.getFiles("responses/");
        string str = to_string(json_["name"]);
        str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
        path = path + str + to_string(files.size()) + ".json";
        fs.openFile(path, "w+");
        string str_ = json_.dump(1);
        fs.writeFile(str_);
        fs.closeFile();
        json_.clear();
    }
    boost::asio::ip::tcp::socket socket_;
    json json_;
    string response_;
};

class Server {
private:
    boost::asio::io_context io_context_;
    vector<Client> clients_;
    std::mutex client_mutex_;
    void accept_connections(tcp::acceptor& acceptor) {
        while (true) {
            tcp::socket socket(io_context_);

            acceptor.accept(socket);
            //std::thread(&Server::handle_clients, this, std::move(socket)).detach();
            {
                // Защищаем доступ к вектору клиентов
                std::lock_guard<std::mutex> lock(client_mutex_);
                clients_.emplace_back(std::move(socket));
            }
            std::cout << SUCCESSFULLY_COLOR << "Новое подключение." << RESET_COLOR << " Всего клиентов : " << RESET_COLOR << clients_.size() << std::endl;
        }
    }

public:
    Server() {}
    void start()
    {
        try {
            tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), PORT));

            std::cout << SUCCESSFULLY_COLOR << "Сервер запущен на порте " << PORT << RESET_COLOR << endl;
            accept_connections(acceptor);

        }
        catch (std::exception& e) {
            std::cerr << WARNING_COLOR << "Ошибка сервера: " << e.what() << RESET_COLOR << std::endl;
        }
    }
    void sendMessageToAll(const std::string& msg)
    {
        std::lock_guard<std::mutex> lock(client_mutex_);

        for (auto it = clients_.begin(); it != clients_.end(); )
        {
            try
            {
                boost::asio::write(it->getSocket(), boost::asio::buffer(msg + "\n"));
                ++it;  
            }
            catch (const std::exception& e)
            {
                std::cout << WARNING_COLOR << "Ошибка при отправке сообщения. Клиент отключен: " << e.what() << RESET_COLOR << std::endl;
                it = clients_.erase(it);  
            }
        }
    }
    void getResponse()
    {
        vector<std::thread> threads;

        for (auto& client : clients_) {
            threads.push_back(std::thread(&Client::acceptResponse, &client));
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        for (auto& client : clients_) {
            //cout << client.getResponse() << endl;
        }
    }
};

void menu(Server& server)
{
    int input;
    string prefix = "_quest_";
    string quest;
    while (true)
    {
        cout << RESET_COLOR << endl << "1 - Начать голосование\n2 - Создать вопрос для голосования\n3 - Взять имеющийся вопрос для голосования\n";
        cin >> input;

        // Очищаем буфер ввода после ввода числа
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (input)
        {
        case 1:
            cout << "Введите вопрос для голосования: ";
            getline(cin, quest);
            server.sendMessageToAll(prefix + quest);
            server.getResponse();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    Server server;

    std::thread menuThread(menu, std::ref(server));

    server.start();

    if (menuThread.joinable()) {
        menuThread.join();
    }

    return 0;
}