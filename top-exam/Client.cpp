#include "Client.h"
#include "FileSystem.h"
#include "ListFiles.h"

Client::Client(tcp::socket s) : socket_(std::move(s)) { }
string Client::acceptResponse()
{
    try {
        char data[1024];

        size_t length = this->socket_.read_some(boost::asio::buffer(data));
        string response = string(data, length);
        return response;
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

string Client::getName() const
{
    return name;
}