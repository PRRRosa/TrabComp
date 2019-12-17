.LC0:
	.string "%d"
.LC1:
	.string "%f"
.LC2:
	.string "%c"
.LC3:
	.string "%ld"
.LC4:
	.string "."
## TEMP_VAR
	.globl  _TEMPORARYNODE0
	.data 4
	.align 4
	.type _TEMPORARYNODE0, @object
	.size _TEMPORARYNODE0, 4
_TEMPORARYNODE0:
	.long 0
## SYMBOL_LITINT
	.globl  _0
	.data 4
	.align 4
	.type _0, @object
	.size _0, 4
_0:
	.long 0
## SYMBOL_LITINT
	.globl  _1
	.data 4
	.align 4
	.type _1, @object
	.size _1, 4
_1:
	.long 1
## TEMP_VAR
	.globl  _TEMPORARYNODE1
	.data 4
	.align 4
	.type _TEMPORARYNODE1, @object
	.size _TEMPORARYNODE1, 4
_TEMPORARYNODE1:
	.long 0
## SYMBOL_LITINT
	.globl  _50
	.data 4
	.align 4
	.type _50, @object
	.size _50, 4
_50:
	.long 50
## TAC_VAR
	.globl  _x
	.data
	.align 4
	.type _x, @object
	.size _x, 4
_x:
	.long 0
## TAC_BEGINFUN
	.globl	main
main:
##TAC_MOVE INTs
	movl  _1(%rip), %eax
	movl  %eax, _x(%rip)
	movl  $0, %eax
##TAC_LABEL
.TEMPORARYLABEL1:
##Binop
	movl  _x(%rip), %edx
	movl  _50(%rip), %eax
	cmpl  %eax, %edx
	setl  %al
	movzbl  %al, %eax
	movl  %eax, _TEMPORARYNODE0(%rip)
	movl  $0, %eax
##TAC_IFZ
	movl  _TEMPORARYNODE0(%rip), %eax
	testl %eax, %eax
	je .TEMPORARYLABEL0
##TAC_PRINTSTR
	leaq  .LC4(%rip), %rdi
	movl  $0, %eax
	call  printf@PLT
	movl  $0, %eax
##Binop
	movl  _x(%rip), %edx
	movl  _1(%rip), %eax
	addl  %edx, %eax
	movl  %eax, _TEMPORARYNODE1(%rip)
##TAC_MOVE INTs
	movl  _TEMPORARYNODE1(%rip), %eax
	movl  %eax, _x(%rip)
	movl  $0, %eax
##TAC_JUMP
jmp .TEMPORARYLABEL1
##TAC_LABEL
.TEMPORARYLABEL0:
## TAC_ENDFUN
	ret
