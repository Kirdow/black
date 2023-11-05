#include "compiler.h"
#include <fstream>
#include <iostream>

namespace black
{
    void compile_program(const std::vector<Op>& operands, const std::string& filename)
    {
        std::ofstream fs(filename);
        fs << "segment .text\n";
        fs << "log:\n";
        fs << "    mov     r9, -3689348814741910323\n";
        fs << "    sub     rsp, 40\n";
        fs << "    mov     BYTE [rsp+31], 10\n";
        fs << "    lea     rcx, [rsp+30]\n";
        fs << ".L2:\n";
        fs << "    mov     rax, rdi\n";
        fs << "    lea     r8, [rsp+32]\n";
        fs << "    mul     r9\n";
        fs << "    mov     rax, rdi\n";
        fs << "    sub     r8, rcx\n";
        fs << "    shr     rdx, 3\n";
        fs << "    lea     rsi, [rdx+rdx*4]\n";
        fs << "    add     rsi, rsi\n";
        fs << "    sub     rax, rsi\n";
        fs << "    add     eax, 48\n";
        fs << "    mov     BYTE [rcx], al\n";
        fs << "    mov     rax, rdi\n";
        fs << "    mov     rdi, rdx\n";
        fs << "    mov     rdx, rcx\n";
        fs << "    sub     rcx, 1\n";
        fs << "    cmp     rax, 9\n";
        fs << "    ja      .L2\n";
        fs << "    lea     rax, [rsp+32]\n";
        fs << "    mov     edi, 1\n";
        fs << "    sub     rdx, rax\n";
        fs << "    lea     rsi, [rsp+32+rdx]\n";
        fs << "    mov     rdx, r8\n";
        fs << "    mov     rax, 1\n";
        fs << "    syscall\n";
        fs << "    add     rsp, 40\n";
        fs << "    ret\n";

        fs << "global _start\n";
        fs << "_start:\n";
        for (const auto& op : operands)
        {
            switch (op.Type)
            {
            case OpType::PUSH:
                fs << "    ;; PUSH " << op.get_u64() << "\n";
                fs << "    push " << op.get_u64() << "\n";
                break;
            case OpType::LOG:
                fs << "    ;; LOG\n";
                fs << "    pop rdi\n";
                fs << "    call log\n";
                break;
            default:
                std::cerr << "Unreachable OpType: " << op.to_str() << std::endl;
                throw std::runtime_error("unreachable");
            }
        }

        fs << "    mov rax, 60\n";
        fs << "    mov rdi, 0\n";
        fs << "    syscall\n";
    }

    void build_program(const std::vector<Op>& operands, const std::string& filename)
    {
        compile_program(operands, filename + ".asm");
        std::stringstream sstr;
        sstr << "nasm -felf64 " << filename << ".asm";
        std::string command = sstr.str();
        std::cout << "] " << command << std::endl;
        std::system(command.c_str());
        sstr.str("");
        sstr << "ld -o " << filename << " " << filename << ".o";
        command = sstr.str();
        std::cout << "] " << command << std::endl;
        std::system(command.c_str());
    }

    void run_program(const std::string& filename)
    {
        std::stringstream sstr;
        sstr << "./" << filename;
        std::string command = sstr.str();
        std::cout << "] " << command << std::endl;
        std::system(command.c_str());
    }
}
