#pragma once

#include "token.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace black
{
    struct Macro
    {
        std::string Name;
        std::vector<Token> Body;

        Macro() = default;
        Macro(const std::string& name, const std::vector<Token>& body)
            : Name(name), Body(body)
        {}

        inline std::vector<Token> Expand(const std::unordered_map<std::string, Macro>& macros, int32_t max_depth) const
        {
            std::vector<Token> result;
            if (max_depth < 0) return result;

            for (const auto& value : Body)
            {
                if (value.Type != TokenType::SYM)
                {
                    result.push_back(value);
                    continue;
                }

                if (macros.find(value.get_string()) == macros.end())
                {
                    result.push_back(value);
                    continue;
                }

                const auto& macro = macros.at(value.get_string());
                auto expanded = macro.Expand(macros, max_depth < 1);
                for (const auto& append : expanded)
                    result.push_back(append);
            }

            return result;
        }
    };
}
