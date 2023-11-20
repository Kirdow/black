#include "pch.h"
#include "source.h"
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
        return result;
    }

    static std::filesystem::path get_file_name(const std::filesystem::path& filepath, const std::string& name)
    {
        if (std::filesystem::exists(filepath.parent_path() / name)) return filepath.parent_path() / name;
        std::filesystem::path std_dir = s_ParentDir.parent_path() / "std";
        return std_dir / name;
    }

    static inline size_t load_code_file(Source& source, size_t index, const std::filesystem::path& filepath, std::unordered_set<std::filesystem::path>& filemap)
    {
        std::filesystem::path absolute = std::filesystem::absolute(filepath);
        if (filemap.find(absolute) != filemap.end()) return 0;
        filemap.insert(absolute);
        std::istringstream input(read_file(filepath));
        std::vector<SourceLine> result;
        SourceSection section;
        section.FilePath = filepath;
        section.Index = index;

        size_t line_count = 0;

        for (std::string line; std::getline(input, line);)
        {
            if (line.find("inc ") == 0)
            {
                section.Lines = result;
                size_t curr_index = line_count + index;
                source.Sections.push_back(section);
                std::stringstream sstr;
                sstr << line.substr(4) << ".bk";
                size_t len = load_code_file(source, curr_index, get_file_name(filepath, sstr.str()), filemap);
                section.Index += source.Sections.at(source.Sections.size() - 1).Index + len;
            }
			else if (strutil::cltrim(line).find("//") == 0)
            {
                line_count++;
                continue;
            }
            else
            {
                SourceLine sLine;
                sLine.Text = line;
                sLine.Line = line_count + 1;
                result.push_back(sLine);
            }

            line_count++;
        }


        section.Lines = result;
        source.Sections.push_back(section);
        return line_count;
    }

    static inline std::vector<std::string> get_code_words(std::string text)
    {
        strutil::trim(text);
        std::vector<std::string> words = strutil::explode(text, " \t\r\n");
        return words;
    }

    static inline std::vector<SourceToken> get_code_words_and_strings(Source& source)
    {
        std::vector<SourceToken> result;
        for (auto& section : source.Sections)
        {
            for (auto& sourceLine : section.Lines)
            {
                std::vector<std::string> code = get_code_words(sourceLine.Text);
                std::vector<std::string> code_result;
                while (!code.empty())
                {
                    std::string value = code.at(0); code.erase(code.begin());
                    std::string str = strutil::fetch_string(value, code);
                    code_result.push_back(str);
                }

                size_t index = 1;
                for (auto& code_token : code_result)
                {
                    SourceToken sToken;
                    sToken.Text = code_token;
                    sToken.Loc.File = section.FilePath.string();
                    sToken.Loc.Line = sourceLine.Line;
                    sToken.Loc.Column = index++;
                    result.push_back(sToken);
                }
            }
        }

        return result;
    }

    std::vector<SourceToken> load_code(const std::filesystem::path& filepath)
    {
        std::unordered_set<std::filesystem::path> filemap;
        Source source;
        load_code_file(source, 0, filepath, filemap);
#if 0 // Enable to log file lines
        std::cout << "Sections: " << source.Sections.size() << std::endl;
        size_t i = 0;
        for (auto& section : source.Sections)
        {
            std::cout << "-- " << i++ << std::endl;
            std::cout << "  Path: " << section.FilePath.string() << std::endl;
            std::cout << "  Index: " << section.Index << std::endl;
            std::cout << "  Text: " << std::endl;
            for (auto& line : section.Lines)
            {
                std::stringstream sstr;
                sstr << line.Line;
                std::string text = sstr.str();
                std::cout << line.Line;
                size_t i = 5 - text.length();
                while (i-- > 0) std::cout << " ";
                std::cout << line.Text << std::endl;
            }
        }
#endif
        std::vector<SourceToken> code_tokens = get_code_words_and_strings(source);
        return code_tokens;
    }
}
