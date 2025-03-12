#include "Server.h"
#define PORT 60

void Server::accept_connections(tcp::acceptor& acceptor)
{
    while (true) {
        tcp::socket socket(io_context_);

        acceptor.accept(socket);
        {
            std::lock_guard<std::mutex> lock(client_mutex_);
            if (countClient < 2) { clients_.emplace_back(std::move(socket));countClient++; }
            else try { boost::asio::write(socket, boost::asio::buffer("Нет мест для подключения \n")); socket.cancel(); }
            catch (exception& ex) { cerr << ex.what() << endl; }
            if (countClient == 2) { break; return; }
        }
        std::cout << endl << SUCCESSFULLY_COLOR << "Новое подключение." << RESET_COLOR << " Всего клиентов : " << RESET_COLOR << clients_.size() << std::endl;
    }
}
void Server::start()
{
    try {
        tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), PORT));

        std::cout << SUCCESSFULLY_COLOR << "Сервер запущен на порте " << PORT << RESET_COLOR << endl;
        accept_connections(acceptor);
        game();
    }
    catch (std::exception& e) {
        std::cerr << WARNING_COLOR << "Ошибка сервера: " << e.what() << RESET_COLOR << std::endl;
    }
}
void Server::sendMessageToAll(const std::string& msg)
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
void Server::sendMessage(Client& client, const string& msg)
{
    std::lock_guard<std::mutex> lock(client_mutex_);
    boost::asio::write(client.getSocket(), boost::asio::buffer(msg + "\n"));
}

json Server::getResponse(Client& client_)
{
    string response = client_.acceptResponse();
    json j = json::parse(response);
    return j;
    
}

void Server::game()
{
    Game game;
    int queue = 0;
    if (true)
    {
        json j;
        j["myMap"] = game.getMap(1, false);
        j["flag"] = 0;
        j["enemyMap"] = game.getMap(0, true);
        j["message"] = "";
        string js = j.dump();

        sendMessage(clients_[1], js);
    }
    while (true)
    {
        json j;
        j["myMap"] = game.getMap(queue, false);
        j["flag"] = 1;
        j["enemyMap"] = game.getMap(queue == 0 ? 1 : 0, true);
        j["message"] = "";
        string js = j.dump();

        sendMessage(clients_[queue], js);
        json response = getResponse(clients_[queue]);
        int x = response["x"];
        int y = response["y"];
        bool successfully = game.hit(queue, x, y);
        j = nullptr;
        js = "";
        j["myMap"] = game.getMap(queue, false);
        j["flag"] = 0;
        j["enemyMap"] = game.getMap(queue == 0 ? 1 : 0, true);
        j["message"] = successfully==true ? "Popal!" : "Ne popal!";
        js = j.dump();
        sendMessage(clients_[queue], js);
        if(!successfully) queue = queue == 0 ? 1 : 0;
        else
        {
            j = nullptr;
            js = "";
            j["myMap"] = game.getMap(queue == 0 ? 1 : 0, false);
            j["flag"] = 0;
            j["enemyMap"] = game.getMap(queue, true);
            j["message"] = "";
            js = j.dump();
            sendMessage(clients_[queue == 0 ? 1 : 0], js);
        }
        int winner = game.winner();
        if (winner != -1)
        {
            j = nullptr;
            j["flag"] = 2;
            j["message"] = "You win!";
            js = j.dump();
            sendMessage(clients_[winner], js);
            j = nullptr;
            j["flag"] = 2;
            j["message"] = "You lost!";
            js = j.dump();
            sendMessage(clients_[winner == 0 ? 1 : 0], js);
            json save;
            json win;
            json lost;
            win["name"] = clients_[winner].getName();
            win["hits"] = game.remainingShips(winner);

            lost["name"] = clients_[winner == 0 ? 1 : 0].getName();
            lost["hits"] = game.remainingShips(winner == 0 ? 1 : 0);
            
            save["win"] = win;
            save["lost"] = lost;

            FileSystem fs;
            auto now = std::chrono::system_clock::now();

            time_t now_time_t = std::chrono::system_clock::to_time_t(now);
            tm* now_tm = std::localtime(&now_time_t);
            string path = to_string(now_tm->tm_year) + to_string(now_tm->tm_mon) + to_string(now_tm->tm_wday) + to_string(now_tm->tm_hour) + to_string(now_tm->tm_min) + ".txt";
            js = save.dump(4);
            fs.openFile(path,"w+");
            fs.writeFile(js);
            fs.closeFile();
            return;
        }
    }
}