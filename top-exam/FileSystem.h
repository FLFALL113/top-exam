#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

class OpenFile {
protected:
    FILE* open(const string& path, const string& mode);
};

class CloseFile {
protected:
    void close(FILE*& file);
};

class WriteToFile {
protected:
    void write(FILE* file, const string& data);
};

class ReadFromFile {
protected:
    string read(FILE* file);
};

class FileSystem : protected OpenFile, protected CloseFile, protected WriteToFile, protected ReadFromFile {
private:
    FILE* file = nullptr;

public:
    ~FileSystem();

    void openFile(const string& path, const string& mode);

    void closeFile();

    void writeFile(const string& data);

    string readFile();
};