
%{
  #include "tokens.h"
  int lineNumber = 1;
  #include "hash.c"
  int varType=1;
%}

%x COMMENT
DIGIT [0-9]
%x STRING
%x IDENTIFY
%%

"byte" {BEGIN(IDENTIFY); varType=KW_BYTE; return KW_BYTE;}
"int" {BEGIN(IDENTIFY); varType=KW_INT; return KW_INT;}
"long" {BEGIN(IDENTIFY); varType= KW_LONG; return KW_LONG;}
"float" {BEGIN(IDENTIFY); varType=KW_FLOAT; return KW_FLOAT;}
"bool" {BEGIN(IDENTIFY); varType=KW_BOOL; return KW_BOOL;}



{DIGIT}+ {hashInsert(yytext,LIT_INTEGER);return KW_INT;}
(({DIGIT}+"."{DIGIT}+)|("."{DIGIT}+)|({DIGIT}+".")) {hashInsert(yytext,LIT_FLOAT);return KW_FLOAT;}
"TRUE"	{hashInsert(yytext,KW_BOOL);return KW_BOOL;}
"FALSE"	{hashInsert(yytext,KW_BOOL);return KW_BOOL;}
['.']	{hashInsert(yytext,LIT_CHAR);return LIT_CHAR;}
["]([^"]|\\\")*["]	{hashInsert(yytext,LIT_STRING);return LIT_STRING;}


<IDENTIFY>[A-Za-z_][A-Za-z_0-9]* {hashInsert(yytext,varType); BEGIN(INITIAL); return TK_IDENTIFIER;}
<IDENTIFY>[^ A-Za-z0-9]	{BEGIN(INITIAL);}

[A-Za-z_][A-Za-z_0-9]* {return TK_IDENTIFIER;}


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
