	.file	"tst3.c"
	.text
	.comm	q,8,8
	.comm	w,8,8
	.comm	e,8,8
	.globl	f
	.type	f, @function
f:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	$11, -8(%rbp)
	movq	$22, -16(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.comm	a,8,8
	.comm	b,8,8
	.comm	x,8,8
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$0, -8(%rbp)
	movq	$0, x(%rip)
	movq	$0, a(%rip)
	movq	$1, b(%rip)
	movq	$0, -8(%rbp)
	jmp	.L4
.L5:
	movq	b(%rip), %rdx
	movq	a(%rip), %rax
	addq	%rdx, %rax
	movq	%rax, x(%rip)
	movq	b(%rip), %rax
	movq	%rax, a(%rip)
	movq	x(%rip), %rax
	movq	%rax, b(%rip)
	addq	$1, -8(%rbp)
.L4:
	cmpq	$10, -8(%rbp)
	jle	.L5
	movq	b(%rip), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
