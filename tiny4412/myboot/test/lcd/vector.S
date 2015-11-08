
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

.global _end_vector
_end_vector:

.align 4

reset:
	stmfd sp!, {r0-r12, lr}

	bl doReset

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

undefined_instruction:
	stmfd sp!, {r0-r12, lr}

	bl doUndefined

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

svc:
	stmfd sp!, {r0-r12, lr}

	bl doSvc

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

prefetch_abort:
	stmfd sp!, {r0-r12, lr}

	bl doPrefetchAbt

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

data_abort:
	stmfd sp!, {r0-r12, lr}

	bl doDataAbt

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4
	
irq:
	ldr sp, =0x55000000
	stmfd sp!, {r0-r12, lr}

	bl doIrq

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

fiq:
	stmfd sp!, {r0-r12, lr}

	bl doFiq

	ldmfd sp!, {r0-r12, lr}
	subs pc, lr , #4

