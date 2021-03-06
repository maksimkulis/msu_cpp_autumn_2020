#pragma once

#include <string> //std::string
#include <sstream> //std::stringstream
#include <stdexcept> //runtime_error
#include <cctype> //isdigit
#include <vector> //vector
#include <utility> //std::forward


enum class status {
    CHAR,
    OPEN_BRACE,
    DIGIT
};

class MyException: public std::runtime_error
{
public:
    MyException(const char* message)
        : std::runtime_error(message)
    {
    }
};

void unpacking(std::vector<std::string>& vec)
{
}

template<class T>
void unpacking(std::vector<std::string>& vec, const T& arg)
{
    std::stringstream stream;
    stream << arg;
    vec.push_back(stream.str());
}

template<class T, class... Args>
void unpacking(std::vector<std::string>& vec, const T& arg, const Args&... args)
{
    std::stringstream stream;
    stream << arg;
    vec.push_back(stream.str());
    unpacking(vec, args...);
}

template<class... Args>
std::string format(const std::string& str, const Args&... args)
{
    std::vector<std::string> vec;
    std::stringstream stream;
    unpacking(vec, args...);

    auto st = status::CHAR;
    size_t index = 0;
    for(auto c : str) {

        switch(st){
        case status::CHAR:
            if (c == '}') {
                throw MyException("Closing bracket before opening one");
            } else if (c != '{') {
                stream << c;
            } else {
                st = status::OPEN_BRACE;
                index = 0;
            }
            break;

        case status::OPEN_BRACE:
            if (!std::isdigit(c)) {
                throw MyException("Digit expected");
            } else {
                index = c - '0';
                st = status::DIGIT;
            }
            break;

        case status::DIGIT:
            if (std::isdigit(c)) {
                index = index * 10 + c - '0';
            } else if (c == '}') {
                if (index >= vec.size()) {
                    throw MyException("Not enough arguments");
                }
                stream << vec[index];
                st = status::CHAR;
            } else {
                throw MyException("Digit expected");
            }
            break;
        default:
            throw MyException("???");
        }
    }
    if (st == status::OPEN_BRACE || st == status::DIGIT) {
        throw MyException("Bad string");
    }
    return stream.str();
}
