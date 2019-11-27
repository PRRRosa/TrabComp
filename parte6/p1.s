	.file	"t1.c"
	.text

	.globl	_a
	.data
	.align 4
	.type	_a, @object
	.size	_a, 4
_a:
	.long	2


## TAC_BEGINFUN
	.globl	_main
_main:                                  ## @main
	pushq	%rbp
	movq	%rsp, %rbp

## TAC_ENDFUN
	ret
	
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
## versão ubuntu
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax

## TAC_READ
	leaq	a(%rip), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	$0, %eax
## usando c88 (-std=c89)
	subq	$8, %rsp
	leaq	a(%rip), %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	scanf@PLT
	nop
	addq	$8, %rsp

## TAC_CALL
	subq	$8, %rsp
	movl	$0, %eax
	call	f2
	movl	%eax, b(%rip) ##mover valor de retorno para var temporária.
	movl	$0, %eax
	addq	$8, %rsp
	## Foi possivel retirar todos os comandos menos o call, e a chamada em si ainda funciona.

## TAC_MOV
	movl	b(%rip), %eax
	movl	%eax, a(%rip)
	movl	$0, %eax

## TAC_ENDFUN
	popq	%rbp
	retq

## TAC_JUMP
	jmp	.label

## TAC_LABEL
	.label

## TAC_RETURN
	movl	a(%rip), %eax
	ret

.section	__TEXT,__cstring,cstring_literals
printmessage:                                 ## @.str
	.asciz	"OI\n"
printvalue:                                 ## @.str
	.asciz	"%d\n"

.LC0:
	.string	"%d"
	.text
	.globl	main
	.type	main, @function