#pragma once

#include "token.h"
#include <string>
#include <filesystem>
#include <vector>

namespace black
{
    std::vector<std::string> load_code(const std::filesystem::path& filepath);
}
