	.file	"tst1.c"
	.text
	.comm	a,8,8
	.comm	b,8,8
	.comm	x,8,8
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$6, a(%rip)
	movq	$5, b(%rip)
	movq	a(%rip), %rdx
	movq	b(%rip), %rax
	cmpq	%rax, %rdx
	jl	.L2
	movq	$150, x(%rip)
.L2:
	movq	$9, x(%rip)
	movq	x(%rip), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
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
