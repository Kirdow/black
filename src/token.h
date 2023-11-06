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
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        LT,
        GT,
        LTE,
        GTE,
        EQ,
        NEQ,
        LOG,
        IF,
        ELSE,
        WHILE,
        DO,
        END,
        DUP,
        OVER,
        SWAP,
        DROP,
        MEM,
        LOAD,
        STORE
    };

    static inline const char* op_name(OpType type)
    {
        switch (type)
        {
        case OpType::NOP: return "NOP";
        case OpType::PUSH: return "PUSH";
        case OpType::LOG: return "LOG";
        case OpType::ADD: return "ADD";
        case OpType::SUB: return "SUB";
        case OpType::MUL: return "MUL";
        case OpType::DIV: return "DIV";
        case OpType::MOD: return "MOD";
        case OpType::LT: return "LT";
        case OpType::GT: return "GT";
        case OpType::LTE: return "LTE";
        case OpType::GTE: return "GTE";
        case OpType::EQ: return "EQ";
        case OpType::NEQ: return "NEQ";
        case OpType::IF: return "IF";
        case OpType::ELSE: return "ELSE";
        case OpType::WHILE: return "WHILE";
        case OpType::DO: return "DO";
        case OpType::END: return "END";
        case OpType::DUP: return "DUP";
        case OpType::OVER: return "OVER";
        case OpType::SWAP: return "SWAP";
        case OpType::DROP: return "DROP";
        case OpType::MEM: return "MEM";
        case OpType::LOAD: return "LOAD";
        case OpType::STORE: return "STORE";
        default: return "UnknownOp";
        }
    }

    struct Op : public ValueType
    {
        OpType Type;

        inline std::string to_str() const
        {
            std::stringstream sstr;
            sstr << op_name(Type);

            switch (Type)
            {
            case OpType::PUSH: sstr << " " << get_u64(); break;
            case OpType::IF: sstr << " " << get_u64(); break;
            case OpType::ELSE: sstr << " " << get_u64(); break;
            case OpType::DO: sstr << " " << get_u64(); break;
            case OpType::END: sstr << " " << get_u64(); break;
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
