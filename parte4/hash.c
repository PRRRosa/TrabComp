#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
HASH_NODE *Table[HASH_SIZE];

void hashInit() {
  int i;
  for (i = 0; i < HASH_SIZE; ++i) {
    Table[i]=0;
  }
}

int hashAddress(char *text){
  int address = 1;
  int i;
  for(i=0;i<strlen(text);++i){
    address = (address * text[i])% HASH_SIZE + 1;
  }
  return address - 1;
}

HASH_NODE *hashInsert(char *text, int type){
  //printf("Inserindo em hash texto %s de tipo %d\n",text,type);
  HASH_NODE *newNode;
  if((newNode = hashFind(text))){
    return newNode;
  }
  int address = hashAddress(text);
  newNode =(HASH_NODE*) calloc(1,sizeof(HASH_NODE));
  newNode->type = type;
  newNode->text = (char*)calloc(strlen(text)+1,sizeof(char));
  strcpy(newNode->text,text);
  newNode->next = Table[address];
  Table[address] = newNode;
  return newNode;
}
void hashPrint(){
  int i;
  HASH_NODE *node;
  for(i=0;i<HASH_SIZE;i++){
    for(node=Table[i];node;node=node->next){
      printf("table[%d] has %s, type is %d\n", i,node->text,node->type);
    }
  }
}

HASH_NODE *hashFind(char *text){
  int address = hashAddress(text);
  HASH_NODE *node;
    for(node=Table[address];node;node=node->next){
      if(!strcmp(text,node->text)){
        return node;
      }
    }
    return 0;
}

void hashCheckUndeclared(){
  int i;
  HASH_NODE *node;
  for(i=0;i<HASH_SIZE;i++){
    for(node=Table[i];node;node=node->next){
      if(node->type == SYMBOL_IDENTIFIER){
        fprintf(stderr,"Semantic error : Symbol %s undeclared\n", node->text);
      }
    }
  }
}
