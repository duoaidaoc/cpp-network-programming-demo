#pragma once

#include<string>

class Buffer{
public:
    Buffer();
    ~Buffer();
    void append(const char*, int);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
private:
    std::string buf;
};