#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <variant>

namespace black
{
    enum class TokenType
    {
        NONE,
        NUM,
        TEXT,
        SYM
    };

    static inline const char* TokenName(TokenType type)
    {
        switch (type)
        {
        case TokenType::NUM: return "NUM";
        case TokenType::TEXT: return "TEXT";
        case TokenType::SYM: return "SYM";
        case TokenType::NONE: return "NONE";
        default: return "UnknownType";
        }
    }

    struct ValueType
    {
        std::variant<std::string, uint64_t, uint8_t> Data;

        inline std::string get_string() const
        {
            return std::get<std::string>(Data);
        }

        inline uint64_t get_u64() const
        {
            return std::get<uint64_t>(Data);
        }

        inline uint32_t get_u32() const
        {
            return static_cast<uint32_t>(get_u64());
        }

        inline uint8_t get_u8() const
        {
            return std::get<uint8_t>(Data);
        }
    };

    struct Token : public ValueType
    {
        TokenType Type;
    };

    enum class OpType
    {
        NOP,
        PUSH,
        LOG
    };


    static inline const char* op_name(OpType type)
    {
        switch (type)
        {
        case OpType::NOP: return "NOP";
        case OpType::PUSH: return "PUSH";
        case OpType::LOG: return "LOG";
        default: return "UnknownOp";
        }
    }

#define PUSH(x) sstr << " " << (x); break
#define PUSH64() PUSH(get_u64())
#define PUSH32() PUSH(get_u32())
#define PUSH8() PUSH(get_u8())
#define PUSHS() PUSH(get_string())

    struct Op : public ValueType
    {
        OpType Type;


        inline std::string to_str() const
        {
            std::stringstream sstr;
            sstr << op_name(Type);

            switch (Type)
            {
            case OpType::PUSH: PUSH64();
            default: break;
            }

            return sstr.str();
        }

        static inline Op create(OpType type)
        {
            Op result;
            result.Type = type;
            return result;
        }

        template<typename T>
        static inline Op create_val(OpType type, const T& value)
        {
            Op result = create(type);
            result.Data = value;
            return result;
        }
    };
}
