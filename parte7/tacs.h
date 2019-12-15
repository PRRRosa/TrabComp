#ifndef TACS_HEADER
#define TACS_HEADER
#include "astree.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_MOVE 6
#define TAC_IFZ 7
#define TAC_LABEL 8
#define TAC_JUMP 11
#define TAC_CALL 12
#define TAC_ARG 13
#define TAC_BEGINFUN 14
#define TAC_ENDFUN 15
#define TAC_LE 16
#define TAC_GE 17
#define TAC_EQ 18
#define TAC_DIF 19
#define TAC_LESS 20
#define TAC_GRE 21
#define TAC_OR 22
#define TAC_ARREF 23
#define TAC_RET 24
#define TAC_READ 25
#define TAC_VAR 26
#define TAC_VEC 27
#define TAC_VECINIT 28
#define TAC_ARRWRITE 29
#define TAC_PRINTSTR 30
#define TAC_PRINTEXP 31
#define TAC_ARGDEC 32
#define TAC_VECINITLIST 33

typedef struct tac_node{
  int type;
  HASH_NODE *res;
  HASH_NODE *op1;
  HASH_NODE *op2;
  struct tac_node *prev;
  struct tac_node *next;
  int argNumber;
}TAC;
TAC* tacCreate(int type, HASH_NODE *res,HASH_NODE *op1,HASH_NODE *op2, int argNumber);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC* l1, TAC* l2);
TAC* generateCode(AST* ast,HASH_NODE* labelLoopEnd);
void generateASM(TAC* tac, FILE* fout);
void writeFixed(TAC* first, FILE* output);

#endif
