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
    if (argc < 2)
    {
        std::cerr << "Usage: black <filename.bk> [options]" << std::endl;

        return EXIT_FAILURE;
    }

    std::stringstream argstr;

    argstr << argv[0] << " " << argv[1];

    bool is_printsrc = false;
    bool is_verbose = false;
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        argstr << " " << arg;
        if (arg == "--log-source")
        {
            is_printsrc = true;
        }
        else if (arg == "--verbose")
        {
            is_verbose = true;
        }
    }

    std::cout << "] " << argstr.str() << std::endl;

    std::string filename_with_ext = argv[1];
    auto last_index = filename_with_ext.find_last_of('.');
    std::string filename = filename_with_ext.substr(0, last_index);
    
    if (is_verbose) std::cout << "-- Creating tokens" << std::endl;
    std::vector<black::Token> tokens = black::lex_tokens(filename);
    if (is_verbose) std::cout << "-- Creating operands" << std::endl;
    std::vector<black::Op> operands = black::lex_operands(tokens);
    if (is_verbose) std::cout << "-- Building executable" << std::endl;
    black::build_program(operands, filename);
    if (is_verbose) std::cout << "-- Running program" << std::endl;
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

