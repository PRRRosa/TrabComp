
%{
  #include "tokens.h"
  int lineNumber = 1;
  #include "hash.c"
  int varType=1;
  int running = 1;

  void countLinesInString(char* stringToTest,int lengthOfString);
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
"if" {return KW_IF;}
"then" {return KW_THEN;}
"else" {return KW_ELSE;}
"while" {return KW_WHILE;}
"for" {return KW_FOR;}
"read" {return KW_READ;}
"print" {KW_PRINT;}
"return" {KW_RETURN;}
"break" {KW_BREAK;}



{DIGIT}+ {hashInsert(yytext,LIT_INTEGER);return LIT_INTEGER;}
(({DIGIT}+"."{DIGIT}+)|("."{DIGIT}+)|({DIGIT}+".")) {hashInsert(yytext,LIT_FLOAT);return LIT_FLOAT;}
"TRUE"	{hashInsert(yytext,LIT_TRUE);return LIT_TRUE;}
"FALSE"	{hashInsert(yytext,LIT_FALSE);return LIT_FALSE;}
"'"."'"	{hashInsert(yytext,LIT_CHAR);return LIT_CHAR;}
["]([^"]|\\\")*["]	{hashInsert(yytext,LIT_STRING);countLinesInString(yytext,yyleng);return LIT_STRING;}

[\-+*/,;:()\[\]{}*<>=v.~&$#] return (int)yytext[0];
<IDENTIFY>[A-Za-z_][A-Za-z_0-9]* {hashInsert(yytext,varType); BEGIN(INITIAL);return TK_IDENTIFIER;}
<IDENTIFY>"\n"	{printf("linha\n");++lineNumber;BEGIN(INITIAL);}
<IDENTIFY>[^ A-Za-z0-9\n]	{BEGIN(INITIAL);}

[A-Za-z_][A-Za-z_0-9]* {hashInsert(yytext,TK_IDENTIFIER);return TK_IDENTIFIER;}

"<="	{return OPERATOR_LE;}
">="	{return OPERATOR_GE;}
"=="	{return OPERATOR_EQ;}
"!="	{return OPERATOR_DIF;}


"\n" {++lineNumber;}
"//".*


"###" BEGIN(COMMENT);
<COMMENT>"###" {BEGIN(INITIAL);}
<COMMENT>"\n" ++lineNumber;
<COMMENT>.

[^ \t] {return TOKEN_ERROR;}
%%

int yywrap(){
  running = 0;
  return 1;
}
void initMe(){
  hashInit();
}
void countLinesInString(char* stringToTest,int lengthOfString){
	for(int currentChar=0;currentChar<lengthOfString;currentChar++){
		if(stringToTest[currentChar]=='\n'){
			lineNumber++;
		}
	}
	return;
}

int getLineNumber(){
  return lineNumber;
}

int isRunning(){
  return running;

}

#include "main.c"
