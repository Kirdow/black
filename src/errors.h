#pragma once

#include <exception>
#include <sstream>
#include <stdexcept>

namespace black
{
    inline void _throw_message(const std::string& message, const char* file, const char* func, int line_number)
    {
        std::stringstream sstr;
        sstr << file << ";" << func << ":" << line_number << ":\n";
        sstr << message;
        throw std::runtime_error(sstr.str());
    }

    template<typename T>
    inline void _throw_unexpected(const char* message, const T& expected, const char* file, const char* func, int line_number)
    {
        std::stringstream sstr;
        sstr << message << expected;
        _throw_message(sstr.str(), file, func, line_number);
    }
}

#define THROW_UNEXPECTED(msg, expected) ::black::_throw_unexpected(msg, expected, __FILE__, __func__, __LINE__)
#define THROW_MESSAGE(msg) ::black::_throw_message(msg, __FILE__, __func__, __LINE__)

