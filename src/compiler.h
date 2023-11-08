#pragma once

#include "token.h"
#include <vector>
namespace black
{
    void compile_program(const std::vector<Op>& operands);
    void build_program(const std::vector<Op>& operands, const std::string& filename);
    int run_program(const std::string& filename);
}
