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
            if (c == '-' && i == 0) continue;
            if (!std::isdigit(c)) return false;
        }

        return true;
    }

    static std::vector<std::string> get_words(const std::string& source)
    {
        std::vector<std::string> words;
        std::string word;
        std::istringstream ts(source);

        while (std::getline(ts, word, ' '))
        {
            strutil::trim(word);
            if (word.empty()) continue;

            words.push_back(word);
        }

        return words;
    }

    std::vector<Token> lex_tokens(const std::string &source)
    {
        std::vector<Token> result;

        std::vector<std::string> words = get_words(source);

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
