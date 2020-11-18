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
    CLOSE_BRACE,
    DIGIT
};

void unpacking(std::stringstream& stream, std::vector<std::string>& vec)
{
}

template<class T>
void unpacking(std::stringstream& stream, std::vector<std::string>& vec, T&& arg)
{
    stream << arg;
    vec.push_back(stream.str());
    stream.str("");
}

template<class T, class... Args>
void unpacking(std::stringstream& stream, std::vector<std::string>& vec, T&& arg, Args&&... args)
{
    stream << arg;
    vec.push_back(stream.str());
    stream.str("");
    unpacking(stream, vec, std::forward<Args>(args)...);
}

template<class... Args>
std::string format(const std::string& str, Args&&... args)
{
    std::vector<std::string> vec;
    std::stringstream stream;
    unpacking(stream, vec, std::forward<Args>(args)...);

    auto st = status::CHAR;
    size_t index = 0;
    for(auto c : str) {

        switch(st){
        case status::CHAR:
            if (c == '}') {
                throw std::runtime_error("Closing bracket before opening one");
            } else if (c != '{') {
                stream << c;
            } else {
                st = status::OPEN_BRACE;
                index = 0;
            }
            break;

        case status::OPEN_BRACE:
            if (!std::isdigit(c)) {
                throw std::runtime_error("Digit expected");
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
                    throw std::runtime_error("Not enough arguments");
                }
                stream << vec[index];
                st = status::CHAR;
            } else {
                throw std::runtime_error("Digit expected");
            }
            break;
        default:
            throw std::runtime_error("???");
        }
    }
    if (st == status::OPEN_BRACE || st == status::DIGIT) {
        throw std::runtime_error("Bad string");
    }
    return stream.str();
}
