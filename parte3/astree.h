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
typedef struct astree_node{
  int type;
  char *text;
  HASH_NODE *symbol;
  struct astree_node *son[MAX_SONS];
}AST;

AST *astreeCreate(int type,HASH_NODE *symbol, AST *s0,AST *s1,AST *s2,AST *s3);
void astreePrint(AST *s0, int level);
//END
