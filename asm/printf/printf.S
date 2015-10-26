

.global _start
_start:
	
	stmfd sp!, {r0-r12, lr}

	bl print

	@以下三行相当于上一行
	@mov lr, pc 
	@b print
	@nop

	ldmfd sp!, {r0-r12, pc}

print:
	stmfd sp!, {r0-r12, lr}

	@uboot printf() address
	ldr r1, =0x43E11434

	ldr r0, =str

	mov lr, pc
	mov pc, r1


	ldmfd sp!, {r0-r12, pc}

str:
	.string "hello world \n"
	.align 4
