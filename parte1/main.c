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
      case KW_BOOL:
        fprintf(stderr, "Found KW bool\n");
        break;
      case KW_FLOAT:
        fprintf(stderr, "Found KW float\n");
        break;
      case KW_LONG:
        fprintf(stderr, "Found KW long\n");
        break;
      case KW_BYTE:
        fprintf(stderr, "Found KW byte\n");
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
  fprintf(stderr, "%i\n", getLineNumber());
}
