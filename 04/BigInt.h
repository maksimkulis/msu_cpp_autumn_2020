#pragma once

#include <iostream>
#include <string>

class BigInt
{
public:
    BigInt();
    BigInt(const BigInt& other);
    BigInt(BigInt&& other);
    BigInt(const std::string& str);
    BigInt(int value);
    ~BigInt();
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other);
    BigInt& operator=(const std::string& str);
    BigInt& operator=(int value);

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator-() const;
    BigInt operator*(const BigInt& other) const;

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    friend std::ostream& operator<<(std::ostream& os, const BigInt& bigint);

private:
    char* _data = nullptr;
    char _sign;
    size_t _capacity;
    size_t _size;

private:
    void init_zero();
    void init_int(int value);
    void init_string(const std::string& str);
    bool abs_greater(const BigInt& other) const;
    BigInt add_bigint(const BigInt& other) const;
    BigInt sub_bigint(const BigInt& other) const;
};