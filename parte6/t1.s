	.text
	.globl _a
	.data
	.align 2
_a:
	.long	1065353216
	.globl _b
	.align 2
_b:
	.long	2
	.globl _c
	.zerofill __DATA,__pu_bss2,_c,4,2
	.cstring
lC0:
	.ascii "%d\12\0"
	.text
	.globl _main
_main:
LFB1:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	movl	_b(%rip), %eax
	cvtsi2ssl	%eax, %xmm1
	movss	_a(%rip), %xmm0
	addss	%xmm1, %xmm0
	cvttss2sil	%xmm0, %eax
	movl	%eax, _c(%rip)
	movss	_a(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	lC0(%rip), %rdi
	movl	$1, %eax
	call	_printf
	movl	$0, %eax
	popq	%rbp
LCFI2:
	ret
LFE1:
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0
	.byte	0x1
	.ascii "zR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x1
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB1-.
	.set L$set$2,LFE1-LFB1
	.quad L$set$2
	.byte	0
	.byte	0x4
	.set L$set$3,LCFI0-LFB1
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$5,LCFI2-LCFI1
	.long L$set$5
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.align 3
LEFDE1:
	.ident	"GCC: (Homebrew GCC 9.2.0_1) 9.2.0"
	.subsections_via_symbols
