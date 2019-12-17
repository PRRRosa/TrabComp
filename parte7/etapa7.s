.LC0:
	.string "%d"
.LC1:
	.string "%f"
.LC2:
	.string "%c"
.LC3:
	.string "%ld"
## TAC_VAR
	.globl  _b
	.data
	.align 4
	.type _b, @object
	.size _b, 4
_b:
	.long 0
## TAC_VAR
	.globl  _a
	.data
	.align 4
	.type _a, @object
	.size _a, 4
_a:
	.long 0
## TEMP_VAR
	.globl  _TEMPORARYNODE0
	.data 4
	.align 4
	.type _TEMPORARYNODE0, @object
	.size _TEMPORARYNODE0, 4
_TEMPORARYNODE0:
	.long 0
## SYMBOL_LITINT
	.globl  _10
	.data 4
	.align 4
	.type _10, @object
	.size _10, 4
_10:
	.long 10
## SYMBOL_LITINT
	.globl  _20
	.data 4
	.align 4
	.type _20, @object
	.size _20, 4
_20:
	.long 20
## SYMBOL_LITINT
	.globl  _6667
	.data 4
	.align 4
	.type _6667, @object
	.size _6667, 4
_6667:
	.long 6667
## TAC_VAR
	.globl  _x
	.data
	.align 4
	.type _x, @object
	.size _x, 4
_x:
	.long 10
## TAC_VAR
	.globl  _y
	.data
	.align 4
	.type _y, @object
	.size _y, 4
_y:
	.long 20
## TAC_VAR
	.globl  _k
	.data
	.align 4
	.type _k, @object
	.size _k, 4
_k:
	.long 6667
## TAC_BEGINFUN
	.globl	funct
funct:
##Binop
	movl  _x(%rip), %edx
	movl  _y(%rip), %eax
	addl  %edx, %eax
	movl  %eax, _TEMPORARYNODE0(%rip)
##TAC_MOVE INTs
	movl  _TEMPORARYNODE0(%rip), %eax
	movl  %eax, _y(%rip)
	movl  $0, %eax
## TAC_ENDFUN
	ret
