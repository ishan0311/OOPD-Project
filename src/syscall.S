    .text
    .globl _start
    .globl syscall3
    .extern main

_start:
    call main

    # exit(0) using syscall3(number=60, arg1=0, arg2=0, arg3=0)
    mov $60, %rdi        # syscall number for exit
    xor %rsi, %rsi       # status 0
    xor %rdx, %rdx
    xor %rcx, %rcx
    call syscall3

# long syscall3(long number, long arg1, long arg2, long arg3);
# rdi=number, rsi=arg1, rdx=arg2, rcx=arg3
# Linux syscall ABI: rax=number, rdi=arg1, rsi=arg2, rdx=arg3
syscall3:
    mov %rdi, %rax       # syscall number
    mov %rsi, %rdi       # arg1
    mov %rdx, %rsi       # arg2
    mov %rcx, %rdx       # arg3
    syscall
    ret

    .globl syscall6

# long syscall6(long number, long a1, long a2, long a3, long a4, long a5, long a6);
# rdi=number, rsi=a1, rdx=a2, rcx=a3, r8=a4, r9=a5, [rsp+8]=a6
# Linux syscall ABI: rax=number, rdi=a1, rsi=a2, rdx=a3, r10=a4, r8=a5, r9=a6
syscall6:
    mov %rdi, %rax       # syscall number
    mov %rsi, %rdi       # arg1
    mov %rdx, %rsi       # arg2
    mov %rcx, %rdx       # arg3
    mov %r8,  %r10       # arg4
    mov %r9,  %r8        # arg5
    mov 8(%rsp), %r9     # arg6 from stack
    syscall
    ret

    .globl syscall
syscall:
    jmp syscall6

    .section .note.GNU-stack,"",@progbits
