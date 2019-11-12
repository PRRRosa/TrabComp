	.section	__TEXT,__text,regular,pure_instructions

## TAC_ADD
movl	_x(%rip), %eax
addl	_y(%rip), %eax
movl	%eax, _TEMPORARYNODE1(%rip)
