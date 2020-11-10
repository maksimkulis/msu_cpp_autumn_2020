#include "BigInt.h"

#include <stdlib.h>

BigInt::BigInt()
        : _sign(1)
        , _capacity(1)
        , _size(1)
{
    _data = static_cast<char*>(malloc(1 * sizeof(char)));
    _data[0] = 0;
}

BigInt::BigInt(const BigInt& other)
        : _sign(other._sign)
        , _capacity(other._capacity)
        , _size(other._size)
{
    _data = static_cast<char*>(malloc(_capacity * sizeof(char)));
    for (size_t i = 0; i < _size; ++i) 
    {
        _data[i] = other._data[i];
    }
}

BigInt::BigInt(BigInt&& other)
        : _data(other._data)
        , _sign(other._sign)
        , _capacity(other._capacity)
        , _size(other._size)
{
    other._data = nullptr;
}

BigInt::BigInt(int value)
        : _sign(value >= 0 ? 1 : -1)
        , _size(1)
        , _capacity(1)
{
    init_int(value);
}

BigInt::BigInt(const std::string& str)
{
    init_string(str);
}


BigInt::~BigInt()
{
    free(_data);
}

BigInt& BigInt::operator=(const BigInt& other)
{
    if (&other == this) {
        return *this;
    }
    _sign = other._sign;
    _capacity = other._capacity;
    _size = other._size;
    char* new_data = static_cast<char*>(malloc(_capacity * sizeof(char)));
    for (size_t i = 0; i < _size; ++i) {
        new_data[i] = other._data[i];
    }
    free(_data);
    _data = new_data;
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other)
{
    _sign = other._sign;
    _capacity = other._capacity;
    _size = other._size;
    _data = other._data;
    other._data = nullptr;
    return *this;
}

BigInt& BigInt::operator=(int value)
{
    _sign = (value >= 0 ? 1 : -1);
    _size = 1;
    _capacity = 1;
    init_int(value);
    return *this;
}

BigInt& BigInt::operator=(const std::string& str)
{
    init_string(str);
    return *this;
}

BigInt BigInt::operator+(const BigInt& other) const
{
    if (_sign == other._sign) {
        return add_bigint(other);
    }
    BigInt tmp = sub_bigint(other);
    tmp._sign *= _sign;
    return tmp;
}

BigInt BigInt::operator-(const BigInt& other) const
{
    if (_sign != other._sign) {
        return add_bigint(other);
    } else {
        BigInt tmp = sub_bigint(other);
        tmp._sign *= _sign;
        return tmp;
    }
}

BigInt BigInt::operator-() const
{
    BigInt res(*this);
    res._sign *= -1;
    return res;
}

BigInt BigInt::operator*(const BigInt& other) const
{
    if ((*this == 0) || other == 0) {
        return BigInt(0);
    }
    BigInt res;
    res._capacity = _size + other._size;
    res._data = static_cast<char*>(realloc(res._data, res._capacity * sizeof(char)));
    for (size_t i = 0; i < res._capacity; ++i) {
        res._data[i] = 0;
    }

    for (size_t j = 0; j < other._size; ++j) {
        size_t tmp = 0;
        size_t i = 0;
        for (i = 0; i < _size; ++i) {
            res._data[j + i] += (other._data[j] * _data[i] + tmp);
            tmp = res._data[j + i] / 10;
            res._data[j + i] %= 10;
        }
        while (tmp) {
            res._data[j + i] += tmp;
            tmp = res._data[j + i] / 10;
            res._data[j + i] %= 10;
            ++i;
        }
    }
    size_t j = res._capacity - 1;
    while (res._data[j] == 0) {
        --j;
    }
    res._size = j + 1;
    return res;
}

bool BigInt::operator==(const BigInt& other) const
{
    if (_size != other._size) return false;
    if ((_size == 1) && (_data[0] == 0) && (other._data[0] == 0)) return true;
    if (_sign != other._sign) return false;
    for (size_t i = 0; i < _size; ++i) {
        if (_data[i] != other._data[i]) return false;
    }
    return true;
}

bool BigInt::operator!=(const BigInt& other) const
{
    return !(*this == other);
}


bool BigInt::operator>(const BigInt& other) const
{
    if (*this == other) return false;
    if (_sign != other._sign) return _sign > other._sign;
    bool greater = abs_greater(other);
    return (_sign == 1 ? greater : !greater);
}

bool BigInt::operator>=(const BigInt& other) const
{
    if (*this == other) return true;
    if (_sign != other._sign) return _sign > other._sign;
    bool greater = abs_greater(other);
    return (_sign == 1 ? greater : !greater);
}

bool BigInt::operator<(const BigInt& other) const
{
    return !(*this >= other);
}

bool BigInt::operator<=(const BigInt& other) const
{
    return !(*this > other);
}

std::ostream& operator<<(std::ostream& os, const BigInt& bigint)
{
    if (bigint._sign == -1) {
        os << '-';
    }
    size_t i = bigint._size - 1;
    do
    {
        os << static_cast<int>(bigint._data[i]);
    }
    while (i-- > 0);
    return os;
}


void BigInt::init_int(int value)
{
    if (value == 0) {
        _data = static_cast<char*>(malloc(sizeof(char)));
        _data[0] = 0;
        return;
    }
    value *= _sign;
    int tmp = value;
    while (tmp / 10 != 0)
    {
        ++_size;
        ++_capacity;
        tmp /= 10;
    }
    _data = static_cast<char*>(malloc(_capacity * sizeof(char)));
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = value % 10;
        value /= 10;
    }
}

void BigInt::init_string(const std::string& str)
{
    if (str.size() == 0) {
        _size = _capacity = 1;
        _data =  static_cast<char*>(malloc(sizeof(char)));
        _data[0] = 0;
        return;
    }
    _sign = (str[0] == '-' ? -1 : 1);

    if (_sign == 1) {
        _size = _capacity = str.size();
        _data =  static_cast<char*>(malloc(_capacity * sizeof(char)));
        for (size_t i = 0; i < str.size(); ++i) {
            _data[i] = str[str.size() - 1 - i] - '0';
        }
    } else {
        _size = _capacity = str.size() - 1;
        _data =  static_cast<char*>(malloc(_capacity * sizeof(char)));
        for (size_t i = 0; i < str.size() - 1; ++i) {
            _data[i] = str[str.size() - 1 - i] - '0';
        }
    }
}

BigInt BigInt::add_bigint(const BigInt& other) const
{
    const BigInt* greater;
    const BigInt* less;
    if (_size >= other._size) {
        greater = this;
        less = &other;
    } else {
        greater = &other;
        less = this;
    }
    BigInt value = *greater;
    int transfer = 0;
    if ((greater->_size == less->_size) && (greater->_capacity == greater->_size)) {
        value._data = static_cast<char*>(realloc(value._data, (value._capacity + 1) * sizeof(char)));
        value._data[value._capacity] = 0;
        value._capacity += 1;
    }
    for (size_t i = 0; i < less->_size; ++i) {
        char tmp = greater->_data[i] + less->_data[i];
        tmp += transfer;
        if (tmp >= 10) {
            transfer = 1;
        } else {
            transfer = 0;
        }
        value._data[i] = tmp % 10;
    }
    return value;
}

bool BigInt::abs_greater(const BigInt& other) const
{
    if (_size != other._size) {
        return _size > other._size;
    }
    size_t i = _size - 1;
    do {
        if (_data[i] != other._data[i]) {
            return _data[i] > other._data[i];
        }
    } while (i-- > 0);
    return false;
}

BigInt BigInt::sub_bigint(const BigInt& other) const
{
    const BigInt* greater;
    const BigInt* less;
    if (abs_greater(other)) {
        greater = this;
        less = &other;
    } else {
        greater = &other;
        less = this;
    }
    BigInt value = *greater;
    for (size_t i = 0; i < less->_size; ++i) {
        if (value._data[i] < less->_data[i]) {
            size_t j = i + 1;
            while (value._data[j] == 0) ++j;
            value._data[j] -= 1;
            value._data[i] += 10;
        }
        value._data[i] -= less->_data[i];
    }
    size_t i = value._size - 1;
    do {
        if (value._data[i] == 0) {
            value._size -= 1;
        } else {
            break;
        }
    } while (i-- > 1);
    value._sign = greater == this ? 1 : -1;
    return value;
}
