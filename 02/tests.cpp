#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

#include "TokenParser.h"

// Make some tests
int main()
{
    {
        TokenParser tp;
        tp.ParseString("dfs 89 asdfadf 8879986 fd8sf98d");
        tp.ParseString("");
    }

    {
        TokenParser tp;
        bool start, finish, digit, string;
        start = finish = digit = string = false;

        tp.SetStartCallback([&start] () { start = true; });
        tp.SetDigitTokenCallback([&digit] (int num) { digit = true; });
        tp.SetStringTokenCallback([&string] (const std::string& str) { string = true; });

        tp.ParseString("");
        assert(start && !finish);
        assert(!(digit || string));

        tp.SetFinishCallback([&finish] () { finish = true; });
        tp.ParseString("");
        assert(start && finish);
        assert(!(digit || string));

        tp.ParseString("Hello");
        assert(string && !digit);

        tp.ParseString("123");
        assert(string && digit);
    }

    {
        TokenParser tp;
        int num = 0;
        std::string str = "";
        tp.SetDigitTokenCallback([&num] (int value) { num += value; });
        tp.SetStringTokenCallback([&str] (const std::string& string) { str += string; });
        tp.ParseString("1000 Hello, 488     World!");
        assert(num == 1488);
        assert(str == "Hello,World!");
    }

    {
        TokenParser tp;
        int num;

        tp.SetDigitTokenCallback([&num] (int value) { num = 5; });
        tp.ParseString("123");
        assert(num == 5);

        tp.SetDigitTokenCallback([&num] (int valie) { num = -5; });
        tp.ParseString("123");
        assert(num == -5);

        std::string str;
        tp.SetStringTokenCallback([&str] (const std::string& string) { str = "mail.ru is cool"; });
        tp.ParseString("yep");
        assert(str == "mail.ru is cool");

        tp.SetStringTokenCallback([&str] (const std::string& string) { str = "yandex.ru is not cool"; });
        tp.ParseString("definitely");
        assert(str == "yandex.ru is not cool");
    }

    {
        TokenParser tp;
        int digitCounter;
        int stringCounter;

        tp.SetStartCallback([&digitCounter, &stringCounter] () { digitCounter = stringCounter = 0; });
        tp.SetDigitTokenCallback([&digitCounter] (int value) { ++digitCounter; });
        tp.SetStringTokenCallback([&stringCounter] (const std::string& str) { ++stringCounter; });

        tp.ParseString("228 Hello, my name 322 is Max");
        assert(digitCounter == 2);
        assert(stringCounter == 5);

        tp.ParseString(
            "Patient: Doctor, I have a pain in my eye whenever I drink tea.\n"
            "Doctor: Take the spoon out of the mug before you drink." );
        assert(digitCounter == 0);
        assert(stringCounter == 24);
    }
    
    std::cout << "TESTS OK" << std::endl;
    return 0;
}