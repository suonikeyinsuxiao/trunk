
.global _start_vector
_start_vector:
	b	reset
	b	undefined_instruction
	b   svc
	b   prefetchAbt
	b   dataAbt
	nop
	b   irq
	b   fiq

reset:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000000
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}

undefined_instruction:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000004
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^

svc:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000008
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^

prefetchAbt:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x5100000c
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^

dataAbt:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000010
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^

irq:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000014
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^

fiq:
	stmfd sp!, {r0-r12, lr}

	ldr r0, =0x51000018
	ldr r1, [r0]

	mov lr, pc
	mov pc, r1

	ldmfd sp!, {r0-r12, pc}^




















	b	undefined_instruction
	b   svc
	b   prefetchAbt
	b   dataAbt
	nop
	b   irq
	b   fiq

.global _end_vector
_end_vector:

