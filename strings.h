#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include "trait.h"

namespace sy
{
class Strings
{
public:
    static std::string& str2lower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), 
                [](unsigned char c)
                {
                return std::tolower(c);
                });
        return str;
    }

    static std::string& str2upper(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), 
                [](unsigned char c)
                {
                return std::toupper(c);
                });
        return str;
    }

    template<typename T>
    static void split(T& container, const std::string& str, char delimiter, bool emptyEnable = false)
    {
        std::istringstream is(str);
        std::string s;
        while(std::getline(is, s, delimiter))
        {
            if(!emptyEnable && s.empty())
            {
                continue;
            }
            container.push_back(s);
        }
        if(emptyEnable && str[str.size() - 1] == delimiter)
        {
            container.push_back("");
        }
    }

    static std::string& remove_char(std::string& str, char character)
    {
        auto iter = remove_if(str.begin(), str.end(), [character](char ch){return ch == character;});
        str.erase(iter, str.end());
        return str;
    }

    static std::string& trim(std::string& str)
    {
        return remove_char(str, ' ');
    }
};

template<typename T1, typename T2, class = typename std::enable_if<Contains<std::string, T1, T2>::value>::type>
std::string operator+(const T1& lhs, const T2& rhs)
{
    std::ostringstream os;
    os << lhs << rhs;
    return os.str();
}

}
