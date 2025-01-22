#include "FileSystem.h"

#define WARNING_COLOR "\033[31m"
#define SUCCESSFULLY_COLOR "\033[32m"
#define RESET_COLOR "\033[0m"

FILE* OpenFile::open(const string& path, const string& mode) {
    FILE* file = fopen(path.c_str(), mode.c_str());
    if (!file) {
        cerr << WARNING_COLOR << "Ошибка при открытии файла: " << RESET_COLOR << path << endl;
    }
    return file;
}

void CloseFile::close(FILE*& file) {
    if (file) {
        if (fclose(file) != 0) {
            cerr << WARNING_COLOR << "Ошибка при закрытии файла: " << RESET_COLOR << endl;
        }
        file = nullptr;
    }
}

void WriteToFile::write(FILE* file, const string& data) {
    if (file) {
        fprintf(file, "%s", data.c_str());
    }
    else {
        cerr << WARNING_COLOR << "Ошибка: файл не открыт для записи." << RESET_COLOR << endl;
    }
}

string ReadFromFile::read(FILE* file) {
    if (file) {
        char buffer[1024];
        string result;
        while (fgets(buffer, sizeof(buffer), file)) {
            result += buffer;
        }
        return result;
    }
    else {
        cerr << WARNING_COLOR << "Ошибка: файл не открыт для чтения." << RESET_COLOR << endl;
        return "";
    }
}



FileSystem::~FileSystem() {
    if (file) {
        close(file);
    }
}

void FileSystem::openFile(const string& path, const string& mode) {
    file = open(path, mode);
}

void FileSystem::closeFile() {
    close(file);
}

void FileSystem::writeFile(const string& data) {
    write(file, data);
}

string FileSystem::readFile() {
    return read(file);
}
