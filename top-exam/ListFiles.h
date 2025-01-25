#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

class FileLister {
public:
    static vector<string> getFiles(const string& path);
};