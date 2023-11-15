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
        SYM,
        CHAR
    };

    static inline const char* token_name(TokenType type)
    {
        switch (type)
        {
        case TokenType::NUM: return "NUM";
        case TokenType::TEXT: return "TEXT";
        case TokenType::SYM: return "SYM";
        case TokenType::CHAR: return "CHAR";
        case TokenType::NONE: return "NONE";
        default: return "UnknownType";
        }
    }

    struct ValueType
    {
        std::variant<std::string, uint64_t, int8_t, bool> Data;

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

        inline int8_t get_i8() const
        {
            return std::get<int8_t>(Data);
        }

		inline bool get_bool() const
		{
			return std::get<bool>(Data);
		}

        template<typename T>
        inline bool is_value() const
        {
            return std::holds_alternative<T>(Data);
        }
    };

    struct Token : public ValueType
    {
        TokenType Type;

        inline std::string to_str() const
        {
            std::stringstream sstr;
            sstr << token_name(Type);

            switch (Type)
            {
            case TokenType::SYM: sstr << " (" << get_string() << ")"; break;
            case TokenType::TEXT: sstr << " \"" << get_string() << "\""; break;
            case TokenType::NUM: sstr << " " << get_u64(); break;
            case TokenType::CHAR: sstr << " '" << get_i8() << "'"; break;
            default: break;
            }

            return sstr.str();
        }

        static inline Token create(TokenType type)
        {
            Token token;
            token.Type = type;
            return token;
        }

        template<typename T>
        static inline Token create_val(TokenType type, const T& value)
        {
            Token token = create(type);
            token.Data = value;
            return token;
        }
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
        SHL,
        SHR,
        BAND,
        BOR,
        BXOR,
        BNOT,
        LT,
        GT,
        LTE,
        GTE,
        EQ,
        NEQ,
        LOG,
        LAND,
        LOR,
        LNOT,
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
        STORE,
        SYSCALL,
        PUTS,
		CAST
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
        case OpType::SHL: return "SHL";
        case OpType::SHR: return "SHR";
        case OpType::BAND: return "BAND";
        case OpType::BOR: return "BOR";
        case OpType::BXOR: return "BXOR";
        case OpType::BNOT: return "BNOT";
        case OpType::LT: return "LT";
        case OpType::GT: return "GT";
        case OpType::LTE: return "LTE";
        case OpType::GTE: return "GTE";
        case OpType::EQ: return "EQ";
        case OpType::NEQ: return "NEQ";
        case OpType::LAND: return "LAND";
        case OpType::LOR: return "LOR";
        case OpType::LNOT: return "LNOT";
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
        case OpType::SYSCALL: return "SYSCALL";
        case OpType::PUTS: return "PUTS";
		case OpType::CAST: return "CAST";
        default: return "UnknownOp";
        }
    }

	static inline const char* op_code(OpType type)
	{
		switch (type)
		{
		case OpType::NOP: return "nop";
		case OpType::PUSH: return "push";
		case OpType::LOG: return ".";
		case OpType::ADD: return "+";
		case OpType::SUB: return "-";
		case OpType::MUL: return "*";
		case OpType::DIV: return "/";
		case OpType::MOD: return "%";
		case OpType::SHL: return "<<";
		case OpType::SHR: return ">>";
		case OpType::BAND: return "&";
		case OpType::BOR: return "|";
		case OpType::BXOR: return "^";
		case OpType::BNOT: return "~";
		case OpType::LT: return "<";
		case OpType::GT: return ">";
		case OpType::LTE: return "<=";
		case OpType::GTE: return ">=";
		case OpType::EQ: return "=";
		case OpType::NEQ: return "!=";
		case OpType::LAND: return "&&";
		case OpType::LOR: return "||";
		case OpType::LNOT: return "!";
		case OpType::IF: return "if";
		case OpType::ELSE: return "else";
		case OpType::WHILE: return "while";
		case OpType::DO: return "do";
		case OpType::END: return "end";
		case OpType::DUP: return "dup";
		case OpType::OVER: return "over";
		case OpType::SWAP: return "swap";
		case OpType::DROP: return "drop";
		case OpType::MEM: return "@";
		case OpType::LOAD: return "L";
		case OpType::STORE: return "S";
		case OpType::SYSCALL: return "syscall";
		case OpType::PUTS: return "p";
		case OpType::CAST: return "cast";
		default: return "unknown_operand";
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
			case OpType::PUSH: {
				sstr << " ";
				if (is_value<std::string>())
					sstr << '"' << get_string() << '"';
				else if (is_value<int8_t>())
					sstr << "'" << (char)get_i8() << "'";
				else if (is_value<uint64_t>())
					sstr << get_u64();
				else if (is_value<bool>())
					sstr << (get_bool() ? "true" : "false");
				else
					sstr << "unknown_type";
				break;
			}
            case OpType::IF: sstr << " " << get_u64(); break;
            case OpType::ELSE: sstr << " " << get_u64(); break;
            case OpType::DO: sstr << " " << get_u64(); break;
            case OpType::END: sstr << " " << get_u64(); break;
            case OpType::PUTS: sstr << " " << (get_i8() != 0 ? "true" : "false"); break;
			case OpType::CAST: sstr << " " << get_string(); break;
			case OpType::SYSCALL: sstr << get_i8(); break;
            default: break;
            }

            return sstr.str();
        }

		inline std::string to_code() const
		{
			std::stringstream sstr;

			switch (Type)
			{
			case OpType::PUSH:
				if (is_value<std::string>())
					sstr << '"' << get_string() << '"';
				else if (is_value<int8_t>())
					sstr << "'" << (char)get_i8() << "'";
				else if (is_value<uint64_t>())
					sstr << get_u64();
				else if (is_value<bool>())
					sstr << (get_bool() ? "true" : "false");
				else
					sstr << "unknown_type";
				break;
			case OpType::CAST: sstr << op_code(Type) << "(" << get_string() << ")"; break;
			case OpType::SYSCALL: sstr << op_code(Type) << get_i8(); break;
			default:
				return op_code(Type);
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
