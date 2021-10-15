; Note Assembly

GLOBAL _main

section .text
global start

func:
    xor eax, eax
    mov eax, 10
    add eax, 5
    ret

_main:
call func
    mov ebx, eax

mov rax, 0x2000004
mov rdi, 1
mov rsi, text
mov rdx, 14
syscall

mov rax, 0x2000004
mov rdi, 1
mov rsi, hello_world
mov rdx, 13
syscall

mov rax, 0x2000004
mov rdi, 1
mov rsi, pgl
mov rdx, 24
syscall

mov rax, 0x2000004
mov rdi, 1
mov rsi, name
mov rdx, 14
syscall

mov rax, 0x2000001
mov rdi, 0
syscall

section .data
text db "sample string", 0x0a
hello_world db "Hello World!", 0x0a
pgl db "T# Programming Language", 0x0a
name db "Ibuki Yoshida", 0x0a
