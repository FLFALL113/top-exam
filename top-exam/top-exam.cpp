#include <boost/asio.hpp>
#include <iostream>
#include <string>

#define PORT 60

using boost::asio::ip::tcp;
using namespace std;

int main() {
    try {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));
        cout << "Server is running on port " << PORT << endl;

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            char buffer[1024];
            size_t length = socket.read_some(boost::asio::buffer(buffer));
            string received_data(buffer, length);

            cout << "Received from client: " << received_data << endl;

            string response = "Hi,client!" + received_data;
            boost::asio::write(socket, boost::asio::buffer(response));
        }
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}