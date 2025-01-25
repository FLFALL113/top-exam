#include "Server.h"
#define PORT 60

void Server::accept_connections(tcp::acceptor& acceptor)
{
    while (true) {
        tcp::socket socket(io_context_);

        acceptor.accept(socket);
        //std::thread(&Server::handle_clients, this, std::move(socket)).detach();
        {
            // Защищаем доступ к вектору клиентов
            std::lock_guard<std::mutex> lock(client_mutex_);
            clients_.emplace_back(std::move(socket));
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
void Server::setQuestion(const std::string& quest)
{
    question = quest;
    for (auto& it : clients_)
    {
        it.question = quest;
    }
}
std::unordered_map<std::string, int> mostResponse(vector<Client>& clients)
{
    cout << "==================================================================" << endl;
    std::unordered_map<std::string, int> responseCount;

    for (int i = 0;i < clients.size();i++) {
        responseCount[clients[i].getResponse()]++;
    }

    int maxCount = 0;
    for (const auto& pair : responseCount) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
        }
    }
    for (const auto& it : responseCount)
    {
        cout << "- " << it.second << " - " << (responseCount.size() / it.second)*25 << "%" << endl;
    }
    std::vector<std::string> mostPopularResponses;
    for (const auto& pair : responseCount) {
        if (pair.second == maxCount) {
            mostPopularResponses.push_back(pair.first);
        }
    }

    std::cout << "Самые популярные ответы (встречаются " << maxCount << " раз):\n";
    for (const auto& response : mostPopularResponses) {
        std::cout << "- " << response << "\n";
    }
    return responseCount;
}
void Server::getResponse()
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
    json j;
    std::unordered_map<std::string, int> _mostResponse = mostResponse(clients_);

    j["clients"] = json::array();
    for (auto& client : clients_) {
        json clientJson = client.getJson();
        j["clients"].push_back({
            {"name", clientJson["name"]},
            {"response", clientJson["message"]}
            });

    }
    j["question"] = this->question;
    j["mostResponse"] = json::array();
    for (const auto& response : _mostResponse)
    {
        j["mostResponse"].push_back({
                    {"response", response.first},
                    {"count", response.second}
            });
    }
    string str = j.dump(1);
    FileSystem fs;
    FileLister fl;
    vector<string> files = fl.getFiles("general_responses/");
    fs.openFile("general_responses/" + to_string(files.size()) +".json", "w+");
    fs.writeFile(str);
    fs.closeFile();
}
