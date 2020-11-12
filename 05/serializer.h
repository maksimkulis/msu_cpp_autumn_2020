#pragma once

#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(const T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }

private:
    std::ostream& out_;
    template <class T, class... Args>
    Error process(T arg, Args... args) {
        if (_save(arg) != Error::NoError) {
            return Error::CorruptedArchive;
        } else {
            return process(args...);
        }
    }
    template <class T>
    Error process(T arg) {
        if (_save(arg) != Error::NoError) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error _save(bool arg)
    {
        if (arg) {
            out_ << "true" << Serializer::Separator;
        } else {
            out_ << "false" << Serializer::Separator;
        }
        return Error::NoError;
    }
    Error _save(uint64_t arg)
    {
        out_ << arg << Serializer::Separator;
        return Error::NoError;
    }
    // process использует variadic templates
};


class Deserializer
{
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    Error load(bool& value)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error load(uint64_t& value)
    {
        std::string str;
        in_ >> str;
        value = 0;
        for (auto c : str) {
            if (value > value * 10 + c - '0') {
                return Error::CorruptedArchive;
            }
            value = value * 10 + c - '0';
        }
        return Error::NoError;
    }

private:
    std::istream& in_;
};
