#include "source.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "strutil.h"

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

    static inline std::string load_code_file(const std::filesystem::path& filepath)
    {
        std::istringstream input(read_file(filepath));
        std::stringstream result;

        for (std::string line; std::getline(input, line);)
        {
            if (line.find("inc ") == 0)
            {
                std::stringstream sstr;
                sstr << line.substr(4) << ".bk";
                std::string sub_text = load_code_file(filepath.parent_path() / sstr.str());
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
        std::string text = load_code_file(filepath);
        std::vector<std::string> code_tokens = get_code_words_and_strings(text);
        return code_tokens;
    }
}