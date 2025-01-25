#include "ListFiles.h"
#include <vector>

std::vector<std::string> FileLister::getFiles(const std::string& path) {
    std::vector<std::string> files;

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry.status())) {
                std::string filename = entry.path().filename().string();
                std::string base_filename = filename.substr(0, filename.size() - 10);
                files.push_back(base_filename);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return files;
}
std::vector<std::string> FileLister::getFiles(const std::string& path,char s) {
    std::vector<std::string> files;

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (fs::is_regular_file(entry.status())) {
                std::string filename = entry.path().filename().string();
                std::string base_filename = filename.substr(0, filename.size() - 5);
                files.push_back(base_filename);
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return files;
}