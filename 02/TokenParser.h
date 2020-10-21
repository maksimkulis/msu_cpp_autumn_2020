#include <iostream>
#include <functional>


class TokenParser
{
public:
    TokenParser();
    void SetStartCallback(std::function<void()>);
    void SetFinishCallback(std::function<void()>);
    void SetDigitTokenCallback(std::function<void(int)>);
    void SetStringTokenCallback(std::function<void(std::string)>);

    void ParseStream(std::istream& is);
    void ParseString(const std::string& str);

private:
    std::function<void()> StartCallback;
    std::function<void()> FinishCallback;
    std::function<void(int)> DigitTokenCallback;
    std::function<void(std::string)> StringTokenCallback;

    bool IsNumber(const std::string& str);
    int StringToInt(const std::string& str);
};
