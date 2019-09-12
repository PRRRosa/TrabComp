#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
int yyparse();
extern FILE* yyin;
int main(int argc, char **argv) {
  initMe();
  if (argc<2){
    exit(1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin == 0){
    exit(2);
  }

  yyparse();

  hashPrint();
  fprintf(stderr, "NOICE\n");
  exit(0);
}
