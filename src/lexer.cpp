#include "lexer.h"
#include "token.h"
#include "strutil.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ranges>
#include <cctype>

namespace black
{
    static bool is_number(const std::string& s)
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            char c = s.at(i);
            if (c == '-' && i == 0 && s.length() > 1) continue;
            if (!std::isdigit(c)) return false;
        }

        return true;
    }

    static std::vector<std::string> explode(const std::string& s, const std::string& delimiters)
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

    std::vector<Token> lex_tokens(const std::string &source)
    {
        std::vector<Token> result;

        std::vector<std::string> words = explode(source, " \t\b\r\n");

        size_t i = 0;
        for (const std::string& word : words)
        {
            Token token;
            if (is_number(word))
            {
                token.Type = TokenType::NUM;
                token.Data = static_cast<uint64_t>(std::stoi(word));
            }
            else
            {
                token.Type = TokenType::SYM;
                token.Data = word;
            }

            result.push_back(token);
        }

        return result;
    }

    template<typename T>
    static void throw_unexpected(const char* message, const T& expected)
    {
        std::stringstream sstr;
        sstr << message << expected;
        throw std::runtime_error(sstr.str());
    }

    std::vector<Op> lex_operands(const std::vector<Token>& tokens)
    {
        std::vector<Op> result;

        for (const auto& token : tokens)
        {
            if (token.Type == TokenType::NUM)
            {
                result.push_back(Op::create_val(OpType::PUSH, token.get_u64()));
                continue;
            }
            else if (token.Type == TokenType::SYM)
            {
                std::string sym = token.get_string();

                if ("." == sym)
                {
                    result.push_back(Op::create(OpType::LOG));
                    continue;
                }
                else if ("+" == sym)
                {
                    result.push_back(Op::create(OpType::ADD));
                    continue;
                }
                else if ("-" == sym)
                {
                    result.push_back(Op::create(OpType::SUB));
                    continue;
                }
                else
                {
                    throw_unexpected("Unexpected symbol: ", sym);
                }
            }
            else
            {
                throw_unexpected("Unexpected token: ", TokenName(token.Type));
            }
        }

        return result;
    }

}
