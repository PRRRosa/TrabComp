
%{
  #include "tokens.h"
  int lineNumber = 1;
  #include "hash.c"
%}

%x COMMENT
DIGIT [0-9]
%x STRING
%%

"int" {return KW_INT;}
[A-Za-z_][A-Za-z_0-9]* {hashInsert(yytext);return TK_IDENTIFIER;}
{DIGIT}+ {return LIT_INTEGER;}
{DIGIT}+"."{DIGIT}+ {return LIT_FLOAT;}
[-+*/] return yytext[0];
"("
")"
";"
[ \t]
"\n" {++lineNumber;}
"//".*


"###" BEGIN(COMMENT);
<COMMENT>"###" {BEGIN(INITIAL);}
<COMMENT>"\n" ++lineNumber;
<COMMENT>.

"\"" BEGIN(STRING);
<STRING>"\\\""
<STRING>"\"" {return LIT_STRING;}
<STRING>"\n" ++lineNumber;
<STRING>.

.

%%
int running = 1;
int yywrap(){
  running = 0;
  return 1;
}
void initMe(){
  hashInit();
}

#include "main.c"
