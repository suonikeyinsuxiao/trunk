.section .text 

data:
.byte 0x11

.align 2

.globl _start
_start:
movl data, %ebx
movl $1, %eax
int $0x80
