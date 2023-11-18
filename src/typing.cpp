#include "pch.h"
#include "typing.h"
#include "strutil.h"

#include "token.h"

namespace black
{
	enum class StaticType
	{
		Int, Ptr, Bool
	};

	static inline const char* type_name(StaticType type)
	{
		switch (type)
		{
		case StaticType::Int: return "Int";
		case StaticType::Ptr: return "Ptr";
		case StaticType::Bool: return "Bool";
		}

		return "UnknownType";
	}

	template<typename T>
	struct TypeStack
	{
		std::vector<T> stack;

		inline void push(T val)
		{
			stack.push_back(val);
		}

		inline void pop(int count = 1)
		{
			stack.erase(stack.end() - count, stack.end());
		}

		inline T peek(int offset = 1)
		{
			return stack.at(stack.size() - offset);
		}

		inline bool has(size_t size = 1)
		{
			return stack.size() >= size;
		}

		inline size_t size() const { return stack.size(); }
	};

	using StaticStack = TypeStack<StaticType>;

	struct Branch
	{
		StaticStack stack;
		int pass;

		Branch() = default;
		Branch(const StaticStack& stack, int pass)
			: stack(stack), pass(pass) {}

		inline void set_pass(int _pass)
		{
			pass = _pass;
		}

		inline bool match(const StaticStack& _stack) const
		{
			if (stack.size() != _stack.size()) return false;

			for (size_t i = 0; i < stack.size(); ++i)
			{
				if (stack.stack.at(i) != _stack.stack.at(i))
					return false;
			}

			return true;
		}
	};

	using BranchStack = TypeStack<Branch>;

	inline static std::optional<std::string> use_opcode(BranchStack& branch_stack, StaticStack& stack, const Op& operand)
	{
		StaticType v0, v1, v2;
		int8_t i0, i1, i2;
		std::string s0, s1;
		Branch b0, b1;
		switch (operand.Type)
		{
		case OpType::NOP:
			break;
		case OpType::PUSH:
			if (operand.is_value<uint64_t>())
				stack.push(StaticType::Int);
			else if (operand.is_value<std::string>())
			{
				stack.push(StaticType::Int);
				stack.push(StaticType::Ptr);
			}
			else if (operand.is_value<bool>())
				stack.push(StaticType::Bool);
			else
				return "Invalid push operand. Unknown type!";
			break;
		case OpType::ADD:
		case OpType::SUB:
			if (!stack.has(2)) return strutil::concat("Operand requires 2 values on stack: ", operand.to_code());
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v1 == StaticType::Int)
			{
				if (v0 == StaticType::Int)
				{
					stack.pop(2);
					stack.push(StaticType::Int);
					break;
				}
				else if (v0 == StaticType::Ptr)
				{
					stack.pop(2);
					stack.push(StaticType::Ptr);
					break;
				}

				return strutil::concat("Unknown type for ", operand.to_code(), ", expected 'Int <Int|Ptr>'. Got 'Int ", type_name(v0), "'.");
			}
			else if (v1 == StaticType::Ptr)
			{
				if (v0 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected 'Ptr Int'. Got 'Ptr ", type_name(v0), "'.");
				stack.pop(2);
				stack.push(StaticType::Ptr);
				break;
			}

			return strutil::concat("Unknown type for first ", operand.to_code(), " parameter, expected Int or Ptr. Got '", type_name(v1), "'.");
		case OpType::MUL:
		case OpType::DIV:
		case OpType::MOD:
		case OpType::SHL:
		case OpType::SHR:
		case OpType::BAND:
		case OpType::BOR:
		case OpType::BXOR:
			if (!stack.has(2)) return strutil::concat("Operand requires 2 values on stack: ", operand.to_code());
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v1 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int. Got ", type_name(v1), ".");
			if (v0 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int Int. Got Int ", type_name(v0), ".");
			stack.pop(2);
			stack.push(StaticType::Int);
			break;
		case OpType::LT:
		case OpType::GT:
		case OpType::LTE:
		case OpType::GTE:
			if (!stack.has(2)) return strutil::concat("Operand requires 2 values on stack: ", operand.to_code());
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v1 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int. Got ", type_name(v1), ".");
			if (v0 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int Int. Got Int ", type_name(v0), ".");
			stack.pop(2);
			stack.push(StaticType::Bool);
			break;
		case OpType::LAND:
		case OpType::LOR:
			if (!stack.has(2)) return strutil::concat("Operand requires 2 values on stack: ", operand.to_code());
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v1 != StaticType::Bool) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Bool. Got ", type_name(v1), ".");
			if (v0 != StaticType::Bool) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Bool Bool. Got Bool ", type_name(v0), ".");
			stack.pop(2);
			stack.push(StaticType::Bool);
			break;
		case OpType::BNOT:
			if (!stack.has(1)) return strutil::concat("Operand requires 1 value on stack: ", operand.to_code());
			v0 = stack.peek(1);
			if (v0 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int. Got ", type_name(v0), ".");
			stack.pop(1);
			stack.push(StaticType::Int);
			break;
		case OpType::LNOT:
			if (!stack.has(1)) return strutil::concat("Operand requires 1 value on stack: ", operand.to_code());
			v0 = stack.peek(1);
			if (v0 != StaticType::Bool) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Bool. Got ", type_name(v0), ".");
			stack.pop(1);
			stack.push(StaticType::Bool);
			break;
		case OpType::EQ:
		case OpType::NEQ:
			if (!stack.has(2)) return strutil::concat("Operand requires 2 values on stack: ", operand.to_code());
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v0 != v1) return strutil::concat("Operand is ", operand.to_code(), " but the types don't match. Comparison is redundant.");
			stack.pop(2);
			stack.push(StaticType::Bool);
			break;
		case OpType::IF:
		case OpType::DO:
			if (!stack.has(1)) return strutil::concat("Operand requires 1 value on stack: ", operand.to_code());
			v0 = stack.peek(1);
			if (v0 != StaticType::Bool) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Bool. Got ", type_name(v0), ".");
			stack.pop(1);

			branch_stack.push(Branch(stack, 0));
			break;
		case OpType::ELSE:
			if (!branch_stack.has(1)) return strutil::concat(operand.to_code(), " requires an existing branch.");
			b0 = branch_stack.peek(1);
			branch_stack.pop(1);
			if (b0.pass == 0)
			{
				branch_stack.push(Branch(stack, 1));
				stack = b0.stack;
				break;
			}
			else if (b0.pass == 1)
			{
				return strutil::concat(operand.to_code(), " cannot execute coming from an existing ", operand.to_code(), " branch");
			}
			break;
		case OpType::WHILE:
			break;
		case OpType::END:
			if (!branch_stack.has(1)) return strutil::concat(operand.to_code(), " requires an existing branch.");
			b0 = branch_stack.peek(1);
			branch_stack.pop(1);
			if (b0.pass != 0)
			{
				if (!b0.match(stack)) return strutil::concat(operand.to_code(), ": Branch mismatch.");
			}
			break;
		case OpType::DUP:
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires 1 value on stack.");
			v0 = stack.peek(1);
			stack.push(v0);
			break;
		case OpType::OVER:
			if (!stack.has(2)) return strutil::concat(operand.to_code(), " requires 2 values on stack.");
			v1 = stack.peek(2);
			stack.push(v1);
			break;
		case OpType::SWAP:
			if (!stack.has(2)) return strutil::concat(operand.to_code(), " requires 2 values on stack.");
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			stack.pop(2);
			stack.push(v0);
			stack.push(v1);
			break;
		case OpType::DROP:
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires 1 value on stack.");
			stack.pop(1);
			break;
		case OpType::MEM:
			stack.push(StaticType::Ptr);
			break;
		case OpType::LOAD:
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires 1 value on stack.");
			v0 = stack.peek(1);
			if (v0 != StaticType::Ptr) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Ptr. Got ", type_name(v0), ".");
			stack.pop(1);
			stack.push(StaticType::Int);
			break;
		case OpType::STORE:
			if (!stack.has(2)) return strutil::concat(operand.to_code(), " requires 2 values on stack.");
			v0 = stack.peek(1);
			v1 = stack.peek(2);
			if (v0 != StaticType::Ptr) return strutil::concat("Unknown type for ", operand.to_code(), ", expected '<T> Ptr'. Got '<T> ", type_name(v0), "'.");
			stack.pop(2);
			break;
		case OpType::PUTS:
			return strutil::concat(operand.to_code(), " is no longer implemented in compiler. If you see this error, your version isn't stable, and you should update.");
		case OpType::LOG:
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires 1 value on stack.");
			v0 = stack.peek(1);
			if (v0 != StaticType::Int) return strutil::concat("Unknown type for ", operand.to_code(), ", expected Int. Got ", type_name(v0), ".");
			stack.pop(1);
			break;
		case OpType::SYSCALL:
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires at least 1 value on stack.");
			v0 = stack.peek(1);
			if (v0 != StaticType::Int) return strutil::concat(operand.to_code(), " requires Int as top value on stack.");
			stack.pop(1);
			i0 = operand.get_i8();
			if (!stack.has(i0)) return strutil::concat(operand.to_code(), " requires ", (i0 + 1), " values on stack.");
			stack.pop(i0);
			stack.push(StaticType::Int);
			break;
		case OpType::CAST:
			s0 = operand.get_string();
			if (!stack.has(1)) return strutil::concat(operand.to_code(), " requires 1 value on stack.");
			v0 = stack.peek(1);
			stack.pop(1);
			if (s0 == "ptr")
			{
				stack.push(StaticType::Ptr);
				break;
			}
			else if (s0 == "int")
			{
				stack.push(StaticType::Int);
				break;
			}
			else if (s0 == "bool")
			{
				stack.push(StaticType::Bool);
				break;
			}

			return strutil::concat(operand.to_code(), " expects Int, Ptr or Bool type. Got ", s0, ".");
		default:
			break;
		}

		return {};
	}

	StaticTypingResult st_validate(const std::vector<Op>& code)
	{
		StaticStack stack;
		BranchStack branch_stack;

		std::stringstream sstr;
		bool success = true;
		size_t error_count = 0;
		size_t ip = 0;
		while (ip < code.size())
		{
			const Op& operand = code.at(ip++);

			auto result = use_opcode(branch_stack, stack, operand);
			if (result) // If result has a value, it means it has an error message.
			{
				if (error_count == 0)
					sstr << "Operand stack mismatch.\n";
				sstr << "\033[91mError:\033[0m\n\t" << result.value() << "\n";
				success = false;
				++error_count;
			}
		}

		if (stack.size() != 0)
		{
			sstr << "\033[91mError:\033[0m\n\tStack mismatch on program exit: " << stack.size() << "\n";
			success = false;
			++error_count;
		}

		if (error_count > 0)
			sstr << "Validation exit with \033[91m" << error_count << "\033[0m errors.\n";

		if (success)
			return StaticTypingResult::Ok("");

		return StaticTypingResult::Error(sstr.str());
	}
}

