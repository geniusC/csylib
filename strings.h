#pragma once

#include <string>
#include <algorithm>
#include <sstream>

namespace csy
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
    };
}
