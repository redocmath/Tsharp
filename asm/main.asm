; Note for me
GLOBAL _main

section .text
global start

_main:
mov rax, 0x2000004
mov rdi, 1
mov rsi, text
mov rdx, 14
syscall
mov rax, 0x2000001
mov rdi, 0
syscall

section .data
text db "sample string", 0x0a