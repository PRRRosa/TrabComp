#ifndef ASTREE_HEADER
#define ASTREE_HEADER
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#define MAX_SONS 4
#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MUL 4
#define AST_DIV 5
#define AST_LE 6
#define AST_GE 7
#define AST_EQ 8
#define AST_DIF 9
#define AST_LESS 10
#define AST_GRE 11
#define AST_OR 12
#define AST_FUNCALL 13
#define AST_ARGLIST 14
#define AST_ARRELEMENT 15
#define AST_ASSIGNCMD 16
#define AST_PRINT 17
#define AST_PRINTSTR 18 //nao tem
#define AST_IFCMD 19
#define AST_READ 20
#define AST_WHILE 21
#define AST_FOR 22 //nao tem
#define AST_BREAK 23 //nao tem
#define AST_RETURN 24
#define AST_BLOCK 25 //?
#define AST_VARDEC 26
#define AST_FUNDEC 27
#define AST_TYPEBYTE 28
#define AST_TYPEINT 29
#define AST_TYPELONG 30
#define AST_TYPEFLOAT 31
#define AST_TYPEBOOL 32
#define AST_LDEC 33 //?
#define AST_ARRDEC 34 //nao tem
#define AST_LISTINIT 35 //nao tem
#define AST_VARDECLST 36 //ano tem
#define AST_DECPARAM 37 //?
#define AST_LCMD 38 //?
#define BOOL_EXP 39


typedef struct astree_node{
  int type;
  char *text;
  HASH_NODE *symbol;
  int line;
  struct astree_node *son[MAX_SONS];
}AST;

AST *astreeCreate(int type,HASH_NODE *symbol, AST *s0,AST *s1,AST *s2,AST *s3, int line);
void astreePrint(AST *s0, int level);
void astreeWrite(AST *node,FILE *fileToWrite);
#endif
//END
