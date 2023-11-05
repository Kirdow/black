#pragma once

#include <algorithm>
#include <cctype>
#include <locale>

namespace strutil
{
    static inline void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    static inline void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static inline void trim(std::string& s)
    {
        rtrim(s);
        ltrim(s);
    }

    static inline std::string cltrim(std::string s)
    {
        ltrim(s);
        return s;
    }

    static inline std::string crtrim(std::string s)
    {
        rtrim(s);
        return s;
    }

    static inline std::string ctrim(std::string s)
    {
        trim(s);
        return s;
    }
}

