#include "source.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include "strutil.h"

extern std::filesystem::path s_ParentDir;

namespace black
{
    static inline std::string read_file(const std::filesystem::path& filepath)
    {
        std::ifstream fs(filepath);
        if (!fs.is_open())
        {
            std::stringstream sstr;
            sstr << "Failed to open the file: " << filepath.string();
            throw std::runtime_error(sstr.str());
        }

        std::stringstream sstr;
        sstr << fs.rdbuf();
        std::string result = sstr.str();
        strutil::trim(result);
        return result;
    }

    static std::filesystem::path get_file_name(const std::filesystem::path& filepath, const std::string& name)
    {
        if (std::filesystem::exists(filepath.parent_path() / name)) return filepath.parent_path() / name;
        std::filesystem::path std_dir = s_ParentDir.parent_path() / "std";
        return std_dir / name;
    }

    static inline std::string load_code_file(const std::filesystem::path& filepath, std::unordered_set<std::filesystem::path>& filemap)
    {
        std::filesystem::path absolute = std::filesystem::absolute(filepath);
        if (filemap.find(absolute) != filemap.end()) return "";
        filemap.insert(absolute);
        std::istringstream input(read_file(filepath));
        std::stringstream result;

        for (std::string line; std::getline(input, line);)
        {
            if (line.find("inc ") == 0)
            {
                std::stringstream sstr;
                sstr << line.substr(4) << ".bk";
                std::string sub_text = load_code_file(get_file_name(filepath, sstr.str()), filemap);
                result << " " << sub_text;
            }
            else
                result << " " << line;
        }

        std::string result_str = result.str();
        strutil::trim(result_str);
        return result_str;
    }

    static inline std::vector<std::string> get_code_words(std::string text)
    {
        strutil::trim(text);
        std::vector<std::string> words = strutil::explode(text, " ");
        return words;
    }

    static inline std::vector<std::string> get_code_words_and_strings(const std::string& text)
    {
        std::vector<std::string> code = get_code_words(text);
        std::vector<std::string> result;
        while (!code.empty())
        {
            std::string value = code.at(0); code.erase(code.begin());
            std::string str = strutil::fetch_string(value, code);
            result.push_back(str);
        }

        return result;
    }

    std::vector<std::string> load_code(const std::filesystem::path& filepath)
    {
        std::unordered_set<std::filesystem::path> filemap;
        std::string text = load_code_file(filepath, filemap);
        std::vector<std::string> code_tokens = get_code_words_and_strings(text);
        return code_tokens;
    }
}
