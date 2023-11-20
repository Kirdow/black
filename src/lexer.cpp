#include "pch.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"
#include "strutil.h"
#include "source.h"
#include "errors.h"

namespace black
{
    static bool is_number(const std::string& s)
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            char c = s.at(i);
            if (c == '-' && i == 0 && s.length() > 1) continue;
            if (!std::isdigit(c)) return false;
        }

        return true;
    }

    static inline Token convert_string_to_token(const SourceToken& s)
    {
        if (s.Text.find("\"") == 0 && s.Text.rfind("\"") == s.Text.length() - 1)
            return Token::create_val(TokenType::TEXT, s.Loc, s.Text.substr(1, s.Text.length() - 2));
        else if (s.Text.find("'") == 0 && s.Text.rfind("'") == s.Text.length() - 1)
            return Token::create_val(TokenType::CHAR, s.Loc, s.Text.at(1));
        else if (is_number(s.Text))
            return Token::create_val(TokenType::NUM, s.Loc, static_cast<uint64_t>(std::stoll(s.Text)));
        return Token::create_val(TokenType::SYM, s.Loc, s.Text);
    }

    static inline bool is_macro_end(Token token, int32_t& count)
    {
        if (token.Type != TokenType::SYM) return false;

        std::string sym = token.get_string();
        if (sym == "end") {
            if (count == 0) return true;
            count -= 1;
        } else if (sym == "if" || sym == "while") {
            count += 1;
        }

        return false;
    }

    static inline bool get_macro_text(Token token, std::string& result)
    {
        if (token.Type != TokenType::SYM)
            return false;

        result = token.get_string();
        return true;
    }

    static inline bool is_macro_start(Token token)
    {
        if (token.Type != TokenType::SYM)
            return false;

        return token.get_string() == "macro";
    }

    static inline std::vector<Token> load_macros_and_expand(const std::vector<Token>& raw_code)
    {
        std::unordered_map<std::string, Macro> macros;
        std::vector<Token> code;

        auto it = raw_code.begin();
        while (it != raw_code.end())
        {
            auto item = *(it++);

            if (!is_macro_start(item)) {
                code.push_back(item);
                continue;
            }

            if (it == raw_code.end())
                THROW_MESSAGE("Unexpected macro name EOF!");

            auto next = *(it++);
            std::string macro_name;
            if (!get_macro_text(next, macro_name))
                THROW_UNEXPECTED("Unexpected macro name: ", next.to_str());

            std::vector<Token> body;
            bool success = false;
            int32_t count = 0;
            while (it != raw_code.end())
            {
                auto value = *(it++);
                if (is_macro_end(value, count))
                {
                    Macro macro(macro_name, body);
                    macros[macro_name] = macro;
                    success = true;
                    break;
                }

                body.push_back(value);
            }

            if (!success)
                THROW_MESSAGE("Failed to read macro: EOF");
        }

        std::vector<Token> result;

        it = code.begin();
        while (it != code.end())
        {
            auto item = *(it++);
            std::string text;
            if (!get_macro_text(item, text))
            {
                result.push_back(item);
                continue;
            }

            if (macros.find(text) == macros.end())
            {
                result.push_back(item);
                continue;
            }

            const auto& macro = macros.at(text);
            auto expanded = macro.Expand(macros, 8);
            for (const auto& append : expanded)
                result.push_back(append);
        }

        return result;
    }


    std::vector<Token> lex_tokens(const std::filesystem::path& filepath)
    {
        std::string name = filepath.filename();
        std::vector<SourceToken> source_code = load_code(filepath.parent_path() / (name + ".bk"));
        std::vector<Token> result;
        for (const SourceToken& word : source_code)
        {
            result.push_back(convert_string_to_token(word));
        }

        return load_macros_and_expand(result);
    }

    std::vector<Op> lex_operands(const std::vector<Token>& tokens)
    {
        std::vector<Op> result;
        std::vector<uint64_t> stack;

        uint64_t ip = 0;
        for (const auto& token : tokens)
        {
            if (token.Type == TokenType::NUM)
                result.push_back(Op::create_val<uint64_t>(OpType::PUSH, token, token.get_u64()));
            else if (token.Type == TokenType::TEXT)
                result.push_back(Op::create_val<std::string>(OpType::PUSH, token, token.get_string()));
            else if (token.Type == TokenType::CHAR)
                result.push_back(Op::create_val<uint64_t>(OpType::PUSH, token, static_cast<uint64_t>(static_cast<uint8_t>(token.get_i8()))));
            else if (token.Type == TokenType::SYM)
            {
                std::string sym = token.get_string();

                if ("." == sym)
                    result.push_back(Op::create(OpType::LOG, token));
                else if ("+" == sym)
                    result.push_back(Op::create(OpType::ADD, token));
                else if ("-" == sym)
                    result.push_back(Op::create(OpType::SUB, token));
                else if ("*" == sym)
                    result.push_back(Op::create(OpType::MUL, token));
                else if ("/" == sym)
                    result.push_back(Op::create(OpType::DIV, token));
                else if ("%" == sym)
                    result.push_back(Op::create(OpType::MOD, token));
                else if ("<<" == sym)
                    result.push_back(Op::create(OpType::SHL, token));
                else if (">>" == sym)
                    result.push_back(Op::create(OpType::SHR, token));
                else if ("&" == sym)
                    result.push_back(Op::create(OpType::BAND, token));
                else if ("|" == sym)
                    result.push_back(Op::create(OpType::BOR, token));
                else if ("^" == sym)
                    result.push_back(Op::create(OpType::BXOR, token));
                else if ("~" == sym)
                    result.push_back(Op::create(OpType::BNOT, token));
                else if ("<" == sym)
                    result.push_back(Op::create(OpType::LT, token));
                else if (">" == sym)
                    result.push_back(Op::create(OpType::GT, token));
                else if ("<=" == sym)
                    result.push_back(Op::create(OpType::LTE, token));
                else if (">=" == sym)
                    result.push_back(Op::create(OpType::GTE, token));
                else if ("&&" == sym)
                    result.push_back(Op::create(OpType::LAND, token));
                else if ("||" == sym)
                    result.push_back(Op::create(OpType::LOR, token));
                else if ("!" == sym)
                    result.push_back(Op::create(OpType::LNOT, token));
                else if ("=" == sym)
                    result.push_back(Op::create(OpType::EQ, token));
                else if ("!=" == sym)
                    result.push_back(Op::create(OpType::NEQ, token));
				else if ("true" == sym)
					result.push_back(Op::create_val<bool>(OpType::PUSH, token, true));
				else if ("false" == sym)
					result.push_back(Op::create_val<bool>(OpType::PUSH, token, false));
                else if ("if" == sym)
                {
                    stack.push_back(ip);
                    result.push_back(Op::create_val<uint64_t>(OpType::IF, token, 0));
                }
                else if ("else" == sym)
                {
                    uint64_t block_ip = stack.at(stack.size() - 1); stack.pop_back();
                    auto& result_sym = result.at(block_ip);
                    if (result_sym.Type == OpType::IF)
                    {
                        result_sym.Data = static_cast<uint64_t>(ip + 1);
                        stack.push_back(ip);
                        result.push_back(Op::create_val<uint64_t>(OpType::ELSE, token, 0));
                    }
                    else
                        throw std::runtime_error("Unexpected else begin");
                }
                else if ("while" == sym)
                {
                    stack.push_back(ip);
                    result.push_back(Op::create(OpType::WHILE, token));
                }
                else if ("do" == sym)
                {
                    uint64_t while_ip = stack.at(stack.size() - 1); stack.pop_back();
                    stack.push_back(ip);
                    result.push_back(Op::create_val<uint64_t>(OpType::DO, token, while_ip));
                }
                else if ("end" == sym)
                {
                    uint64_t block_ip = stack.at(stack.size() - 1); stack.pop_back();
                    auto& op = result.at(block_ip);
                    if (op.Type == OpType::IF)
                    {
                        op.Data = static_cast<uint64_t>(ip + 1);
                        result.push_back(Op::create_val<uint64_t>(OpType::END, token, ip + 1));
                    }
                    else if (op.Type == OpType::ELSE)
                    {
                        op.Data = static_cast<uint64_t>(ip + 1);
                        result.push_back(Op::create_val<uint64_t>(OpType::END, token, ip + 1));
                    }
                    else if (op.Type == OpType::DO)
                    {
                        uint64_t do_ip = op.get_u64();
                        op.Data = static_cast<uint64_t>(ip + 1);
                        result.push_back(Op::create_val<uint64_t>(OpType::END, token, do_ip));
                    }
                    else 
                        throw std::runtime_error("Unexpected end begin");
                }
                else if ("dup" == sym)
                    result.push_back(Op::create(OpType::DUP, token));
                else if ("over" == sym)
                    result.push_back(Op::create(OpType::OVER, token));
                else if ("swap" == sym)
                    result.push_back(Op::create(OpType::SWAP, token));
                else if ("drop" == sym)
                    result.push_back(Op::create(OpType::DROP, token));
                else if ("store" == sym || "S" == sym)
                    result.push_back(Op::create(OpType::STORE, token));
                else if ("load" == sym || "L" == sym)
                    result.push_back(Op::create(OpType::LOAD, token));
                else if ("@" == sym)
                    result.push_back(Op::create(OpType::MEM, token));
                //else if ("P" == sym)
                //    result.push_back(Op::create_val<int8_t>(OpType::PUTS, token, true));
                //else if ("p" == sym)
                //    result.push_back(Op::create_val<int8_t>(OpType::PUTS, token, false));
                else if ("syscall1" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 1));
                else if ("syscall2" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 2));
                else if ("syscall3" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 3));
                else if ("syscall4" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 4));
                else if ("syscall5" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 5));
                else if ("syscall6" == sym)
                    result.push_back(Op::create_val<int8_t>(OpType::SYSCALL, token, 6));
				else if ("cast(ptr)" == sym)
					result.push_back(Op::create_val<std::string>(OpType::CAST, token, "ptr"));
				else if ("cast(int)" == sym)
					result.push_back(Op::create_val<std::string>(OpType::CAST, token, "int"));
				else if ("cast(bool)" == sym)
					result.push_back(Op::create_val<std::string>(OpType::CAST, token, "bool"));
                else
                    THROW_UNEXPECTED("Unexpected symbol: ", sym);
            }
            else
                THROW_UNEXPECTED("Unexpected token: ", token_name(token.Type));

            ++ip;
        }

        return result;
    }

}
