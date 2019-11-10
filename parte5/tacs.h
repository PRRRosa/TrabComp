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
#define TAC_PRINT 10
#define TAC_JUMP 11
#define TAC_CALL 12
#define TAC_ARG 13
#define TAC_BEGINFUN 14
#define TAC_ENDFUN 15

typedef struct tac_node{
  int type;
  HASH_NODE *res;
  HASH_NODE *op1;
  HASH_NODE *op2;
  struct tac_node *prev;
  struct tac_node *next;
}TAC;
TAC* tacCreate(int type, HASH_NODE *res,HASH_NODE *op1,HASH_NODE *op2);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* tacJoin(TAC* l1, TAC* l2);
TAC* generateCode(AST* ast);
#endif