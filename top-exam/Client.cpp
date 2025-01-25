#include "Client.h"
#include "FileSystem.h"
#include "ListFiles.h"

Client::Client(tcp::socket s) : socket_(std::move(s)) { boost::asio::write(this->socket_, boost::asio::buffer("Ожидайте вопроса.")); }
void Client::acceptResponse()
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
        json_.clear();
        json_ = received_json;
        std::cout << "Человек с именем: " << received_json["name"] << " ответил: " << std::endl;
        std::cout << received_json["message"] << std::endl;
        response_ = received_json["message"];
        cout << endl;
        saveResponse();
    }
    catch (const boost::system::system_error& e) {
        std::cerr << WARNING_COLOR << "Ошибка при чтении ответа от клиента: " << e.what() << RESET_COLOR << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << WARNING_COLOR << "Общая ошибка при обработке ответа от клиента: " << e.what() << RESET_COLOR << std::endl;
    }
}
tcp::socket& Client::getSocket() { return socket_; }
string Client::getResponse() { return response_; }
void Client::saveResponse()
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
}
json Client::getJson() { return json_; }
