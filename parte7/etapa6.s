.LC0:
	.string "%d"
.LC1:
	.string "%f"
.LC2:
	.string "%c"
.LC3:
	.string "%ld"
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
