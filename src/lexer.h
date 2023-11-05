#pragma once

#include "token.h"
#include <vector>

namespace black
{
    std::vector<Token> lex_tokens(const std::string& source);
    std::vector<Op> lex_operands(const std::vector<Token>& tokens);
}
