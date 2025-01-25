#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include "FileSystem.h"
#include "ListFiles.h"
#include <mutex>
#define PORT 60

using boost::asio::ip::tcp;
using namespace std;
using json = nlohmann::json;
using boost::asio::ip::tcp;