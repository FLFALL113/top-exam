#include "Server.h"
#define PORT 60

void Server::accept_connections(tcp::acceptor& acceptor)
{
    while (true) {
        tcp::socket socket(io_context_);

        acceptor.accept(socket);
        {
            // �������� ������ � ������� ��������
            std::lock_guard<std::mutex> lock(client_mutex_);
            if (clients_.size() < 2) { clients_.emplace_back(std::move(socket));countClient++; }
            else try { boost::asio::write(socket, boost::asio::buffer("��� ���� ��� ����������� \n"));socket.cancel(); }
            catch (exception& ex) { cerr << ex.what() << endl; }
        }
        std::cout << endl << SUCCESSFULLY_COLOR << "����� �����������." << RESET_COLOR << " ����� �������� : " << RESET_COLOR << clients_.size() << std::endl;
    }
}
void Server::start()
{
    try {
        tcp::acceptor acceptor(io_context_, tcp::endpoint(tcp::v4(), PORT));

        std::cout << SUCCESSFULLY_COLOR << "������ ������� �� ����� " << PORT << RESET_COLOR << endl;
        accept_connections(acceptor);

    }
    catch (std::exception& e) {
        std::cerr << WARNING_COLOR << "������ �������: " << e.what() << RESET_COLOR << std::endl;
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
            std::cout << WARNING_COLOR << "������ ��� �������� ���������. ������ ��������: " << e.what() << RESET_COLOR << std::endl;
            it = clients_.erase(it);
        }
    }
}
void Server::sendMessage(Client& client, const string& msg)
{
    std::lock_guard<std::mutex> lock(client_mutex_);
    boost::asio::write(client.getSocket(), boost::asio::buffer(msg + "\n"));
}

string Server::getResponse(Client& client_)
{
    string response = client_.acceptResponse();
    return response;
    /*vector<std::thread> threads;

    for (auto& client : clients_) {
        if(&client==&client_)
        {threads.push_back(std::thread(&Client::acceptResponse, &client));}
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }*/
    /*json j;
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
    fs.closeFile();*/
}
