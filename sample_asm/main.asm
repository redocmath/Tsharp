GLOBAL _main

section .data
hello_world db "Hello World!", 0x0a

section .text
global start

_main:
mov rax, 0x2000004
mov rdi, 1
mov rsi, hello_world
mov rdx, 14
syscall
mov rax, 0x2000001
mov rdi, 0
syscall