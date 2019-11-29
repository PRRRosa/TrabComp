#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

exe: yaccObject lexObject mainObject hashObject asTreeObject semanticObject tacsObject
	gcc -o etapa6 lex.yy.o main.o hash.o y.tab.o astree.o semantic.o tacs.o

lexObject: lexScan
	gcc -c lex.yy.c

yaccObject: yaccAnalysis
	gcc -c y.tab.c

hashObject:hash.c
	gcc -c hash.c

mainObject:main.c
	gcc -c main.c

asTreeObject: astree.c
	gcc -c astree.c

semanticObject: semantic.c
	gcc -c semantic.c

tacsObject: tacs.c
	gcc -c tacs.c

lexScan: scanner.l
	lex --header-file=lex.yy.h scanner.l

yaccAnalysis: lexScan
	yacc parser.y -d

clean:
	rm lex.yy.c lex.yy.h etapa6 y.tab.c y.tab.h *.o etapa6.s

geraAsm:
	gcc -S t1.c -fomit-frame-pointer

