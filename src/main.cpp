#include "compiler.h"
#include "lexer.h"
#include "token.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "strutil.h"

std::string read_file(const std::string& filename);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: black <filename.bk>" << std::endl;

        return EXIT_FAILURE;
    }

    std::string filename_with_ext = argv[1];
    auto last_index = filename_with_ext.find_last_of('.');
    std::string filename = filename_with_ext.substr(0, last_index);
    std::string source_file = read_file(filename_with_ext);
    
    std::vector<black::Token> tokens = black::lex_tokens(source_file);
    std::vector<black::Op> operands = black::lex_operands(tokens);
    black::build_program(operands, filename);
    black::run_program(filename);
    
    return 0;
}

std::string read_file(const std::string& filename)
{
    std::ifstream fs(filename);

    if (!fs.is_open())
    {
        std::stringstream sstr;
        sstr << "Failed to open the file: " << filename;
        throw std::runtime_error(sstr.str());
    }

    std::stringstream sstr;
    sstr << fs.rdbuf();
    std::string result = sstr.str();
    strutil::trim(result);
    return result;
}

