#include <iostream>
#include <cassert>
#include <sstream>

#include "serializer.h"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    Data(uint64_t a, bool b, uint64_t c)
        : a(a)
        , b(b)
        , c(c)
    {
    }

    template <class S>
    Error serialize(S& serializer) const
    {
        return serializer(a, b, c);
    }
    template <class D>
    Error deserialize(D& deserializer)
    {
        Error error = Error::NoError;
        if ((error = deserializer.load(a)) != Error::NoError) {
            return error;
        }
        if ((error = deserializer.load(b)) != Error::NoError) {
            return error;
        }
        return deserializer.load(c);
    }
};

int main()
{
    Data x(1, true, 2);
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);

    Data y { 228, false, 911 };
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    assert(err == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);

    {
        auto s = std::stringstream("");
        Deserializer d(s);
        assert(d.load(y) == Error::CorruptedArchive);
    }

    {
        auto s = std::stringstream("1 privet 3");
        Deserializer d(s);
        assert (d.load(y) == Error::CorruptedArchive);
    }

    {
        auto s = std::stringstream("1000 1");
        Deserializer d(s);
        assert (d.load(y) == Error::CorruptedArchive);
    }

    {
        auto s = std::stringstream("100 false 500");
        Deserializer d(s);
        assert (d.load(y) == Error::NoError);

        assert(y.a == 100);
        assert(y.b == false);
        assert(y.c == 500);
    }

    std::cout << "TESTS OK" << std::endl;
    return 0;
}
