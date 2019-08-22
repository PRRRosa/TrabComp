
%{
  #define KW_INT 256
  #define KW_INTEGER 257
  #define IDENTIFIER 258
  #define LIT_INT 259
  #define LIT_REAL 270
  int lineNumber = 1;
}%

%x COMMENT
DIGIT [0-9]

%%

"int" {return KW_INT;}
"integer" {return KW_INTEGER;}
[A-Za-z_][A-Za-z_0-9]* {fprintf(stderr, "lexema=%s\n",yytext );return IDENTIFIER;}
{DIGIT}+ {return LIT_INT;}
{DIGIT}+"."{DIGIT}+ {return LIT_REAL;}
[-+*/]
"("
")"
";"
[ \t]
"\n" {++lineNumber;}
"//".*
.

"/*" BEGIN(COMMENT);
<COMMENT>"*/" BEGIN(INITIAL);
<COMMENT>"\n" ++lineNumber;
<COMMENT>.


%%
int running = 1;
int yywrap(){
  running = 0;
  return 1;
}

int main() {
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
      case KW_INTEGER:
        fprintf(stderr, "Found KW integer\n");
        break;
      case IDENTIFIER:
        fprintf(stderr, "Identifier\n");
        break;
      case LIT_INT:
        fprintf(stderr, "Literal Integer\n");
        break;
      case LIT_REAL:
        fprintf(stderr, "Literal Real\n");
        break;
      default:
        break;
    }
  }
}
