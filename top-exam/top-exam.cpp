#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <nlohmann/json.hpp>
#include <cstdlib>

#define PORT 60

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;
using boost::asio::ip::tcp;

struct Question {
    int id;
    string question_text;
    vector<string> answers;
    vector<string> names;
};
vector<tcp::socket> clients;
vector<Question> questions;

void handle_client(tcp::socket socket) {
    try {
        clients.push_back(socket);
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");

        istream stream(&buffer);
        string data;
        getline(stream, data);

        json received_json = json::parse(data);

        string name = received_json["name"];
        string answer = received_json["answer"];

        for (auto& question : questions) {
            if (question.id == received_json["question_id"]) {
                question.answers.push_back(answer);
                break;
            }
        }

        for (auto& question : questions) {
            if (question.answers.size() == 2) {  
                json response = { {"question_id", question.id},
                                 {"question_text", question.question_text} };
                string response_str = response.dump();
                boost::asio::write(socket, boost::asio::buffer(response_str + "\n"));
                break;
            }
        }
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void Server()
{
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));
        cout << "Server is running on port " << PORT << endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            thread(handle_client, move(socket)).detach();
        }
    }
    catch (exception& e) {
        cerr << "Server error: " << e.what() << endl;
    }
}

void CreateVoting()
{
    Question q;
    string quest;
    q.id = questions[questions.size() - 1].id + 1;
    cout << "Введите вопрос: ";
    cin >> quest;
    q.question_text = quest;
    questions.push_back(q);
}

void StartMenu()
{
    while (true)
    {
        int choice;
        cout << "1 - Запуск сервера\n 2 - Создать голосование\n 3 - \n 4 - \n 5 - \n" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            thread(Server).detach();
        case 2:
            CreateVoting();
        }
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    
    StartMenu();
    return 0;
}