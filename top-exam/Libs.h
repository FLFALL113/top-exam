#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>

#define WARNING_COLOR "\033[31m"
#define SUCCESSFULLY_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

using boost::asio::ip::tcp;
using namespace std;
