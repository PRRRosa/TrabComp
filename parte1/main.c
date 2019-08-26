int main() {
  initMe();
  int tok;
  while (isRunning()) {
    tok = yylex();
    if(!isRunning()){
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
      case LIT_TRUE:
        fprintf(stderr, "Literal True\n");
        break;
      case LIT_FALSE:
        fprintf(stderr, "Literal False\n");
        break;
      case LIT_CHAR:
        fprintf(stderr, "Literal Char\n");
        break;
      case KW_IF:
        fprintf(stderr, "Found KW if\n");
        break;
      case KW_THEN:
        fprintf(stderr, "Found KW then\n");
        break;
      case KW_ELSE:
        fprintf(stderr, "Found KW else\n");
        break;
      case KW_WHILE:
        fprintf(stderr, "Found KW while\n");
        break;
      case KW_FOR:
        fprintf(stderr, "Found KW for\n");
        break;
      case KW_READ:
        fprintf(stderr, "Found KW read\n");
        break;
      case KW_PRINT:
        fprintf(stderr, "Found KW print\n");
        break;
      case KW_RETURN:
        fprintf(stderr, "Found KW return\n");
        break;
      case KW_BREAK:
        fprintf(stderr, "Found KW break\n");
        break;
      case TOKEN_ERROR:
        fprintf(stderr, "Token error\n");
        break;
      default:
        fprintf(stderr, "ascii token %i\n", tok);
        break;
    }
  }
  hashPrint();
  fprintf(stderr, "%i\n", getLineNumber());
}
