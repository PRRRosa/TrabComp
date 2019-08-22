int main() {
  initMe();
  int tok;
  while (running) {
    tok = yylex();
    if(!running){
      break;
    }
    switch (tok) {
      case KW_INT:
        fprintf(stderr, "Found KW int\n");
        break;
      case TK_IDENTIFIER:
        fprintf(stderr, "Identifier\n");
        break;
      case LIT_INTEGER:
        fprintf(stderr, "Literal Integer\n");
        break;
      case LIT_FLOAT:
        fprintf(stderr, "Literal Float\n");
        break;
      case LIT_STRING:
        fprintf(stderr, "Literal String\n");
        break;
      default:
        fprintf(stderr, "ascii token %c\n", tok);
        break;
    }
  }
  hashPrint();
}
