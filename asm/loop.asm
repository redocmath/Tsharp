; this is a note for me.
; Prints 0 ~ 10
GLOBAL _main

extern _printf
section .text

_main:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov r12, 0

loop_head:
    cmp r12, 11
    jne loop_body

    add rsp, 16
    pop rbp
    ret

loop_body:
    mov rdi, str
    mov rsi, r12
    call _printf

    inc r12
    jmp loop_head

    section .data
str: db "%d"
     db 10 0	
