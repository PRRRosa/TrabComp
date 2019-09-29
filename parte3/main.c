#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "main.h"
int yyparse();
void initMe();
extern FILE* yyin;
int main(int argc, char **argv) {
  initMe();
  if (argc<3){
    exit(1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin == 0){
    exit(2);
  }
  yyout = fopen(argv[2],"w");
  yyparse();

  //hashPrint();

  printf("\nFINISHED\n");
  exit(0);
}
