	.text
	.comm	e,8,8
	.comm	w,8,8
	.comm	q,8,8
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
	movq	$1, %rax
	pushq	%rax
	movq	%rax, q(%rip)
	movq	$2, %rax
	pushq	%rax
	movq	%rax, w(%rip)
	movq	$3, %rax
	pushq	%rax
	movq	%rax, e(%rip)
	movq	w(%rip), %rax
	pushq	%rax
	movq	e(%rip), %rax
	popq	%rdx
	imulq	%rdx, %rax
	pushq	%rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	pushq	%rax
	movq	q(%rip), %rax
	popq	%rdx
	addq	%rdx, %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
