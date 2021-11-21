	.file	"tst1.c"
	.text
	.comm	f_result,8,8
	.comm	a,8,8
	.comm	b,8,8
	.comm	c,8,8
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
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movq	%r9, -48(%rbp)
	addq	$1, -8(%rbp)
	addq	$2, -16(%rbp)
	addq	$3, -24(%rbp)
	addq	$4, -32(%rbp)
	addq	$5, -40(%rbp)
	addq	$6, -48(%rbp)
	addq	$7, 16(%rbp)
	addq	$8, 24(%rbp)
	addq	$9, 32(%rbp)
	addq	$10, 40(%rbp)
	addq	$11, 48(%rbp)
	addq	$12, 56(%rbp)
	addq	$13, 64(%rbp)
	movq	-48(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.globl	g
	.type	g, @function
g:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	$55, -8(%rbp)
	addq	$1, -8(%rbp)
	cmpq	$150, -8(%rbp)
	jne	.L4
	movl	$10, %eax
	jmp	.L5
.L4:
	cmpq	$140, -8(%rbp)
	jne	.L6
	addq	$1, -8(%rbp)
.L6:
	addq	$8, -8(%rbp)
	addq	$77, -8(%rbp)
	movq	-8(%rbp), %rax
.L5:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	g, .-g
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$1, a(%rip)
	pushq	$13
	pushq	$12
	pushq	$11
	pushq	$10
	pushq	$9
	pushq	$8
	pushq	$7
	movl	$6, %r9d
	movl	$5, %r8d
	movl	$4, %ecx
	movl	$3, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	f
	addq	$56, %rsp
	movq	%rax, b(%rip)
	movq	b(%rip), %rax
	movq	%rax, %rdi
	call	g
	movq	%rax, c(%rip)
	movq	a(%rip), %rdx
	movq	b(%rip), %rax
	addq	%rax, %rdx
	movq	c(%rip), %rax
	addq	%rdx, %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
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
