	.file	"tst12.c"
	.text
	.globl	global
	.data
	.align 8
	.type	global, @object
	.size	global, 8
global:
	.quad	2
	.text
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
	movq	$2, -24(%rbp)
	movq	-24(%rbp), %rax
	imulq	%rax, %rax
	imulq	-24(%rbp), %rax
	leaq	(%rax,%rax), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	global(%rip), %rdx
	movq	global(%rip), %rax
	imulq	%rax, %rdx
	movq	global(%rip), %rax
	imulq	%rdx, %rax
	leaq	(%rax,%rax), %rdx
	movq	global(%rip), %rax
	addq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movl	$0, %eax
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
