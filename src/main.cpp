#include "pch.h"
#include "compiler.h"
#include "lexer.h"
#include "token.h"
#include "strutil.h"
#include "typing.h"
#include "meta.h"

std::string read_file(const std::string& filename);

std::filesystem::path s_ParentDir;

int main(int argc, char** argv)
{
	const char* arg_filename = nullptr;
    std::stringstream argstr;
    argstr << argv[0];

    bool is_printsrc = false;
    bool is_verbose = false;
	bool is_validation = true;
	bool is_help = false;
	bool is_version = false;
    for (int i = 1; i < argc; i++)
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
		else if (arg == "--no-type-check")
		{
			is_validation = false;
		}
		else if (arg == "--help")
		{
			is_help = true;
		}
		else if (arg == "--version")
		{
			is_version = true;
		}
		else if (arg_filename == nullptr)
		{
			arg_filename = argv[i];
		}
    }
	
	if (is_version || is_help)
	{
		std::cout << BLACK_NAME << " (" << BLACK_PLATFORM << ") " << BLACK_VERSION << std::endl;
		if (!is_help)
		{
			std::cout << "Copyright (c) " << BLACK_COPY_YEAR << " " << BLACK_COPY_NAME << std::endl;
			std::cout << BLACK_LICENSE_TEXT0 << std::endl;
			std::cout << BLACK_LICENSE_TEXT1 << std::endl;
			return 0;
		}
	}
	
	if (is_help)
	{
		std::cout << "Usage: black <filename.bk> [options]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "  --help\t\t" << "Show this help menu." << std::endl;
		std::cout << "  --log-source\t\t" << "Log the source code before compiling." << std::endl;
		std::cout << "  --no-type-check\t" << "Skip static-typing validation." << std::endl;
		std::cout << "  --verbose\t\t" << "Detailed logging." << std::endl;
		std::cout << "  --version\t\t" << "Show version notice." << std::endl;
		std::cout << "For more help, visit: https://github.com/Kirdow/black" << std::endl;

		return 0;
	}

    if (arg_filename == nullptr)
    {
        std::cerr << "Usage: black [options] filename.bk" << std::endl;

        return EXIT_FAILURE;
    }

    std::filesystem::path current_path = argv[0];
    s_ParentDir = current_path.parent_path();



    std::cout << "] " << argstr.str() << std::endl;

    std::string filename_with_ext = argv[1];
    auto last_index = filename_with_ext.find_last_of('.');
    std::string filename = filename_with_ext.substr(0, last_index);
    
    if (is_verbose) std::cout << "-- Creating tokens" << std::endl;
    std::vector<black::Token> tokens = black::lex_tokens(filename);
    if (is_verbose) std::cout << "-- Creating operands" << std::endl;
    std::vector<black::Op> operands = black::lex_operands(tokens);
	if (is_validation)
	{
		if (is_verbose) std::cout << "-- Validating static typing" << std::endl;
		auto result = black::st_validate(operands, filename);
		if (!result.IsOk())
		{
			std::cout << result.GetMessage() << "\033[91mValidation Failed\033[0m" << std::endl;
			return EXIT_FAILURE;
		}

		if (is_verbose && !result.GetMessage().empty())
			std::cout << "\033[92mValidation Ok:\033[0m" << std::endl << result.GetMessage() << std::endl;
	}
	else if (is_verbose)
		std::cout << "-- Skipping static type validation" << std::endl;
    if (is_verbose) std::cout << "-- Building executable" << std::endl;
    black::build_program(operands, filename);
    if (is_verbose) std::cout << "-- Running program" << std::endl;
    return black::run_program(filename);
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

