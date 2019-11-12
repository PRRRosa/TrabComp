	.section	__DATA,__data
	_a: .long	1
	_b: .long	2

	.section	__TEXT,__text,regular,pure_instructions
## TAC_BEGINFUN
	.globl	_main
_main:                                  ## @main
	pushq	%rbp
	movq	%rsp, %rbp

## TAC_PRINTSTRING
	leaq	printmessage(%rip), %rdi
	movb	$0, %al
	callq	_printf
## TAC_ADD e outros
	movl	_a(%rip), %eax
	addl	_b(%rip), %eax
	movl	%eax, _a(%rip)

## TAC_PRINTVALUE
	movl	_a(%rip), %esi
	leaq	printvalue(%rip), %rdi
	movb	$0, %al
	callq	_printf

## TAC_ENDFUN
	popq	%rbp
	retq

.section	__TEXT,__cstring,cstring_literals
printmessage:                                 ## @.str
	.asciz	"OI\n"
printvalue:                                 ## @.str
	.asciz	"%d\n"
