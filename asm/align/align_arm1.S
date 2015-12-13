
.global _start
_start:
b reset

.byte 0x55
.byte 0xaa
.align

reset:
ldr r0, =0x53000000
