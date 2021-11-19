	.file	"tst2.c"
	.text
	.comm	a,4,4
	.comm	b,4,4
	.comm	x,4,4
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
	movl	$6, a(%rip)
	movl	$5, b(%rip)
	movl	a(%rip), %edx
	movl	b(%rip), %eax
	cmpl	%eax, %edx
	jle	.L2
	movl	$150, x(%rip)
	movl	x(%rip), %eax
	jmp	.L3
.L2:
	movl	$9, x(%rip)
	movl	x(%rip), %eax
.L3:
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
