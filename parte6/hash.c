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
      printf("table[%d] has %s, type is %d, datatype is %d\n", i,node->text,node->type, node->datatype);
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

int hashCheckUndeclared(){
  int i;
  int errors = 0;
  HASH_NODE *node;
  for(i=0;i<HASH_SIZE;i++){
    for(node=Table[i];node;node=node->next){
      if(node->type == SYMBOL_IDENTIFIER){
        fprintf(stderr,"Semantic error : Symbol %s undeclared\n", node->text);
        ++errors;
      }
    }
  }
  return errors;
}
HASH_NODE* makeTemp(){
  static int serialNumber = 0;
  static char name[100];
  printf("Inserindo temp %d\n",serialNumber);
  sprintf(name,"TEMPORARYNODE%d", serialNumber++);
  HASH_NODE* tempNode = hashInsert(name,TEMP_VAR);
  tempNode->datatype = DATATYPE_INT;
  return tempNode;
}
HASH_NODE* makeLabel(){
  static int serialNumber = 0;
  static char name[100];

  sprintf(name,"TEMPORARYLABEL%d", serialNumber++);
  return hashInsert(name,0);
}

void writeVars(FILE *fout){
  int i;
  HASH_NODE *node;
  for(i=0;i<HASH_SIZE;i++){
    for(node=Table[i];node;node=node->next){
      if(node->type == TEMP_VAR){
        fprintf(fout, "## TEMP_VAR\n"
        "\t.globl  _%s\n"
        "\t.data 4\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long 0\n", node->text, node->text, node->text, node->text);
      }else if(node->type == SYMBOL_LITINT){
        int valor = atoi(node->text);
        fprintf(fout, "## SYMBOL_LITINT\n"
        "\t.globl  _%s\n"
        "\t.data 4\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long %d\n", node->text, node->text, node->text, node->text,valor);
      }else if (node->type == SYMBOL_LITREAL){

          volatile float tempFloat = atof(node->text);
          int tempInt = *(int*)&tempFloat;

          fprintf(fout, "## TAC_VAR float\n"
            ".globl\t_%s\n"
            "\t.data\n"
             "\t.align 4\n"
             "\t.type _%s, @object\n"
            "\t.size _%s, 4\n"
          "_%s:\n"
           "\t.long %d\n"
           "##valor real:%f\n", node->text, node->text, node->text,node->text, tempInt,tempFloat);
      }else if ((node->type == SYMBOL_LITTRUE)||(node->type == SYMBOL_LITFALSE)){
        fprintf(fout, "## SYMBOL_LIT%s \n"
        "\t.globl  _%s\n"
        "\t.data 4\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long %d\n", node->text, node->text, node->text, node->text, node->text,node->type == SYMBOL_LITTRUE?1:0);
      }
    }
  }
}

