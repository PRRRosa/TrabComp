int main(int argc, char **argv) {
  //yyin = fopen(argv(1),"c");
  initMe();
  if (argc<2){
    exit(1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin == 0){
    exit(2);
  }
  int tok;

  yyparse();

  hashPrint();
  //fprintf(stderr, "%i\n", getLineNumber());
  fprintf(stderr, "NOICE\n");
  exit(0);
}
