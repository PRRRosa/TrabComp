int main() {
  initMe();
  int tok;

  yyparse();

  hashPrint();
  //fprintf(stderr, "%i\n", getLineNumber());
  fprintf(stderr, "NOICE\n");
  exit(0);
}
