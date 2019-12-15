#ifndef HASH_HEADER
#define HASH_HEADER
#include <stdio.h>
#define HASH_SIZE 997

#define SYMBOL_IDENTIFIER 51
#define SYMBOL_LITINT 52
#define SYMBOL_LITREAL 53

#define SYMBOL_SCALAR 54
#define SYMBOL_VECTOR 55
#define SYMBOL_FUNCTION 56

#define SYMBOL_LITCHAR 57
#define SYMBOL_LITTRUE 58
#define SYMBOL_LITFALSE 59

#define SYMBOL_LABEL 60

#define TEMP_VAR 61


#define DATATYPE_BYTE 1
#define DATATYPE_INT 2
#define DATATYPE_LONG 3
#define DATATYPE_FLOAT 4
#define DATATYPE_BOOL 5

typedef struct hash_node{
  int type;
  int datatype;
  char *text;
  struct hash_node *next;
  struct astree_node *funDeclNode;
}HASH_NODE;

void hashInit();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint();
int hashCheckUndeclared();
HASH_NODE* makeTemp();
HASH_NODE* makeLabel();
void writeVars(FILE *fout);
#endif
//END
