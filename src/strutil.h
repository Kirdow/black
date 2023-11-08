#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

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

    static inline std::string& replace_all(std::string& str, const std::string& from, const std::string& to)
    {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }

        return str;
    }

    static inline std::string creplace_all(std::string str, const std::string& from, const std::string& to)
    {
        replace_all(str, from, to);
        return str;
    }

    static inline std::vector<std::string> explode(const std::string& s, const std::string& delimiters)
    {
        std::vector<std::string> result;
        size_t current;
        size_t next = -1;

        do
        {
            current = next + 1;
            next = s.find_first_of(delimiters, current);
            std::string str = s.substr(current, next - current);
            if (str.empty()) continue;
            result.push_back(str);
        }
        while(next != std::string::npos);

        return result;
    }

    static inline bool is_number(const std::string& s)
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            char c = s.at(i);
            if (c == '-' && i == 0 && s.length() > 1) continue;
            if (!std::isdigit(c)) return false;
        }

        return true;
    }

    struct StrLitMatch
    {
        std::string Goal;
        std::string NoGoal;
        bool End;

        StrLitMatch() = default;
        StrLitMatch(const std::string& value, const std::string& goal, const std::string& no_goal)
            : Goal(goal), NoGoal(no_goal), End(false)
        {
            End = value.rfind(goal) == value.length() - goal.length() && value.rfind(no_goal) != value.length() - no_goal.length() && value.length() >= 2;
        }

        bool ends(const std::string& value) const
        {
            return value.rfind(Goal) == std::abs(static_cast<int64_t>(value.length() - Goal.length())) && value.rfind(NoGoal) != std::abs(static_cast<int64_t>(value.length() - NoGoal.length()));
        }
    };

    static inline std::string format_string_token(std::string text)
    {
        replace_all(text, "\\n", "\n");
        replace_all(text, "\\t", "\t");
        replace_all(text, "\\\"", "\"");
        replace_all(text, "\\\'", "\'");
        replace_all(text, "\\\\", "\\");

        if (text.find("'") == 0)
        {
            if (text.length() != 3)
                throw std::runtime_error("Character typed strings can only have a single character!");
        }

        return text;

    }

    static inline bool check_string_literal(const std::string& value, StrLitMatch& result)
    {
        if (value.find("\"") != 0 && value.find("'") != 0)
        {
            return false;
        }

        std::string goal, no_goal;

        if (value.find("\"") == 0)
        {
            goal = "\"";
            no_goal = "\\\"";
        }
        else
        {
            goal = "'";
            no_goal = "\\'";
        }

        result = StrLitMatch(value, goal, no_goal);
        return true;
    }

    static inline std::string fetch_string(const std::string& value, std::vector<std::string>& code)
    {
        StrLitMatch strlit;
        if (check_string_literal(value, strlit))
        {
            if (strlit.End)
                return format_string_token(value);

            std::stringstream sstr;
            sstr << value;
            while (!code.empty())
            {
                std::string value2 = code.at(0); code.erase(code.begin());
                sstr << " " << value2;
                if (strlit.ends(value2))
                    break;
            }

            return format_string_token(sstr.str());
        }
        
        return value;
    }
}

