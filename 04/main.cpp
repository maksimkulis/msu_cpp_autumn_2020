#include <iostream>
#include <cassert>

#include "BigInt.h"


void comparison()
{
    {
        BigInt a("12345");
        BigInt b("1234");
        assert(!(a == b));
        assert(a != b);
        assert(!(a < b));
        assert(!(a <= b));
        assert(a > b);
        assert(a >= b);
    }
    {
        BigInt a("-12345");
        BigInt b("1234");
        assert(!(a == b));
        assert(a != b);
        assert(a < b);
        assert(a <= b);
        assert(!(a > b));
        assert(!(a >= b));
    }
    {
        BigInt a("12345");
        BigInt b("-1234");
        assert(!(a == b));
        assert(a != b);
        assert(a > b);
        assert(a >= b);
        assert(!(a < b));
        assert(!(a <= b));
    }
    {
        BigInt a("12345");
        BigInt b("12345");
        assert(b == a);
        assert(!(b != a));
        assert(!(a > b));
        assert(a >= b);
        assert(!(a < b));
        assert(a <= b);
    }
    {
        BigInt a("-12345");
        BigInt b("-12345");
        assert(b == a);
        assert(!(b != a));
        assert(!(a > b));
        assert(a >= b);
        assert(!(a < b));
        assert(a <= b);
    }
}

void constructors()
{
    {
        BigInt a;
        assert(a == 0);
    }
    {
        BigInt a("12345");
        assert(a == 12345);
    }
    {
        BigInt a("-12345");
        assert(a == -12345);
    }
    {
        BigInt a(12345);
        assert(a == 12345);
    }
    {
        BigInt a(-12345);
        assert(a == -12345);
    }
    {
        BigInt a(BigInt(12345));
        assert(a == 12345);
    }
    {
        BigInt a(12345);
        BigInt b(a);
        assert(b == 12345);
    }
    {
        BigInt a("000015");
        assert(a == 15);
    }
}

void op_eq()
{
    {
        BigInt a;
        a = 15;
        assert(a == 15);
    }
    {
        BigInt a;
        a = "15";
        assert(a == 15);
    }
    {
        BigInt a;
        BigInt b;
        a = 15;
        b = a;
        assert(b == 15);
    }
    {
        BigInt a;
        a = BigInt(15);
        assert(a == 15);
    }
}

void arithmetic()
{
    {
        BigInt a = 15;
        BigInt b = 20;
        assert(a + b == 35);
        assert(a - b == -5);
        assert(b + a == 35);
        assert(b - a == 5);
    }
    {
        BigInt a = -15;
        BigInt b = 20;
        assert(a + b == 5);
        assert(a - b == -35);
        assert(b + a == 5);
        assert(b - a == 35);
    }
    {
        BigInt a = 15;
        BigInt b = -20;
        assert(a + b == -5);
        assert(a - b == 35);
        assert(b + a == -5);
        assert(b - a == -35);
    }
    {
        BigInt a = -5;
        assert(-a == 5);
        assert(-(-a) == -5);
    }
    {
        BigInt a = 1000;
        BigInt b = 1000;
        assert(a - b == 0);
        assert(a + b == 2000);
    }
    {
        BigInt a("764309785416758326491");
        BigInt b("8965014765174614051483");
        assert(a * b == BigInt("6852048511428679303648228160647868396736153"));
    }
}

int main()
{
    comparison();
    constructors();
    op_eq();
    arithmetic();
    std::cout << "TESTS OK" << std::endl;
    return 0;
}