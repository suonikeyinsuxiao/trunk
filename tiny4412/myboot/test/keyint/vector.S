
.section .text
.align 2
.global _start_vector
_start_vector:
	ldr pc, _reset
	ldr pc, _undefined_instruction
	ldr pc, _svc
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	nop
	ldr pc, _irq
	ldr pc, _fiq

_reset:
	.word reset

_undefined_instruction:
	.word undefined_instruction

_svc:
	.word svc

_prefetch_abort:
	.word prefetch_abort
	
_data_abort:
	.word data_abort

_irq:
	.word irq

_fiq:
	.word fiq

.align 2
.global _end_vector
_end_vector:

reset:
	b reset

undefined_instruction:
	b undefined_instruction

svc:
	b svc

prefetch_abort:
	b prefetch_abort

data_abort:
	b data_abort

irq:
	stmfd sp!, {r0-r12, lr}

	bl doIrq

	ldmfd sp!, {r0-r12, lr}  
	subs pc, lr, #4				

fiq:
	b fiq

