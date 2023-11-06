#include "compiler.h"
#include "token.h"
#include <fstream>
#include <iostream>

namespace black
{
    void compile_program(const std::vector<Op>& operands, const std::string& filename)
    {
        std::ofstream fs(filename);
        fs << "segment .data\n";
        fs << "segment .bss\n";
        fs << "    membuf  resb 640 * 1024\n";
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
        uint64_t ptr = 0;
        for (const auto& op : operands)
        {
            fs << "addr_" << ptr << ":\n";
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
            case OpType::ADD:
                fs << "    ;; ADD\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    add rax, rdx\n";
                fs << "    push rax\n";
                break;
            case OpType::SUB:
                fs << "    ;; SUB\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    sub rax, rdx\n";
                fs << "    push rax\n";
                break;
            case OpType::MUL:
                fs << "    ;; MUL\n";
                fs << "    xor rdx, rdx\n";
                fs << "    pop rcx\n";
                fs << "    pop rax\n";
                fs << "    mul rcx\n";
                fs << "    push rax\n";
                break;
            case OpType::DIV:
                fs << "    ;; DIV\n";
                fs << "    xor rdx, rdx\n";
                fs << "    pop rcx\n";
                fs << "    pop rax\n";
                fs << "    div rcx\n";
                fs << "    push rax\n";
                break;
            case OpType::MOD:
                fs << "    ;; MOD\n";
                fs << "    xor rdx, rdx\n";
                fs << "    pop rcx\n";
                fs << "    pop rax\n";
                fs << "    div rcx\n";
                fs << "    push rdx\n";
                break;
            case OpType::LT:
                fs << "    ;; LT\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmovl rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::GT:
                fs << "    ;; GT\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmovg rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::LTE:
                fs << "    ;; LTE\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmovle rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::GTE:
                fs << "    ;; GTE\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmovge rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::EQ:
                fs << "    ;; EQ\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmove rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::NEQ:
                fs << "    ;; NEQ\n";
                fs << "    xor rcx, rcx\n";
                fs << "    mov rbx, 1\n";
                fs << "    pop rdx\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, rdx\n";
                fs << "    cmovne rcx, rbx\n";
                fs << "    push rcx\n";
                break;
            case OpType::IF:
                fs << "    ;; IF\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, 0\n";
                fs << "    je addr_" << op.get_u64() << "\n";
                break;
            case OpType::ELSE:
                fs << "    ;; ELSE\n";
                fs << "    jmp addr_" << op.get_u64() << "\n";
                break;
            case OpType::DO:
                fs << "    ;; DO\n";
                fs << "    pop rax\n";
                fs << "    cmp rax, 0\n";
                fs << "    je addr_" << op.get_u64() << "\n";
                break;
            case OpType::WHILE:
                fs << "    ;; WHILE\n";
                break;
            case OpType::END:
                fs << "    ;; END\n";
                fs << "    jmp addr_" << op.get_u64() << "\n";
                break;
            case OpType::DUP:
                fs << "    ;; DUP\n";
                fs << "    mov rax, [rsp]\n";
                fs << "    push rax\n";
                break;
            case OpType::OVER:
                fs << "    ;; OVER\n";
                fs << "    mov rax, [rsp+8]\n";
                fs << "    push rax\n";
                break;
            case OpType::SWAP:
                fs << "    ;; SWAP\n";
                fs << "    pop rax\n";
                fs << "    xchg rax, [rsp]\n";
                fs << "    push rax\n";
                break;
            case OpType::DROP:
                fs << "    ;; DROP\n";
                fs << "    pop rax\n";
                break;
            case OpType::MEM:
                fs << "    ;; MEM U64\n";
                fs << "    lea rax, [rel membuf]\n";
                fs << "    push rax\n";
                break;
            case OpType::LOAD:
                fs << "    ;; LOAD\n";
                fs << "    xor rcx, rcx\n";
                fs << "    pop rax\n";
                fs << "    mov cl, [rax]\n";
                fs << "    push rcx\n";
                break;
            case OpType::STORE:
                fs << "    ;; STORE\n";
                fs << "    pop rax\n";
                fs << "    pop rcx\n";
                fs << "    mov [rax], cl\n";
                break;
            default:
                std::cerr << "Unreachable OpType: " << op.to_str() << std::endl;
                throw std::runtime_error("unreachable");
            }

            ++ptr;
        }

        fs << "addr_" << ptr << ":\n";

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
