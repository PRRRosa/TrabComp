#ifndef HASH_HEADER
#define HASH_HEADER
#include <stdio.h>
#define HASH_SIZE 997

#define SYMBOL_IDENTIFIER 1
#define SYMBOL_LITINT 2
#define SYMBOL_LITREAL 3

#define SYMBOL_SCALAR 4
#define SYMBOL_VECTOR 5
#define SYMBOL_FUNCTION 6

#define DATATYPE_BOOL 1
#define DATATYPE_CHAR 2
#define DATATYPE_BYTE 3
#define DATATYPE_INT 4

typedef struct hash_node{
  int type;
  int datatype;
  char *text;
  struct hash_node *next;
}HASH_NODE;

void hashInit();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint();
void hashCheckUndeclared();
#endif
//END
