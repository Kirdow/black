#pragma once

#include "token.h"
#include <string>
#include <filesystem>
#include <vector>

namespace black
{
    struct SourceLine
    {
        std::string Text;
        size_t Line;
    };

    struct SourceSection
    {
        std::filesystem::path FilePath;
        size_t Index;
        std::vector<SourceLine> Lines;
    };

    struct Source
    {
        std::vector<SourceSection> Sections;
    };

    struct SourceToken
    {
        std::string Text;
        Location Loc;
    };

    std::vector<SourceToken> load_code(const std::filesystem::path& filepath);
}
