#include "Client.h"
#include "FileSystem.h"
#include "ListFiles.h"

Client::Client(tcp::socket s) : socket_(std::move(s)) { }
string Client::acceptResponse()
{
    try {
        boost::asio::streambuf buf;

        boost::asio::read_until(this->socket_, buf, '\n');

        std::istream input_stream(&buf);
        std::string response;
        std::getline(input_stream, response);

        if (response.empty()) {
            std::cerr << WARNING_COLOR "�������� ������ ������!" << RESET_COLOR << std::endl;
            return "empty";
        }
        return response;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << WARNING_COLOR << "������ ��� ������ ������ �� �������: " << e.what() << RESET_COLOR << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << WARNING_COLOR << "����� ������ ��� ��������� ������ �� �������: " << e.what() << RESET_COLOR << std::endl;
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
