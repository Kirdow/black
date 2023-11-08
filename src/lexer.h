#pragma once

#include "token.h"
#include <vector>
#include <filesystem>

namespace black
{
    std::vector<Token> lex_tokens(const std::filesystem::path& filepath);
    std::vector<Op> lex_operands(const std::vector<Token>& tokens);
}
