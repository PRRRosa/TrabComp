
%{
  #include "astree.h"
  #include "hash.h"
  #include "y.tab.h"
  int lineNumber = 1;

  int running = 1;
  int getLineNumber(){
    return lineNumber;
  }
  void countLinesInString(char* stringToTest,int lengthOfString);
%}

%x COMMENT
DIGIT [0-9]
%%

"byte" {return KW_BYTE;}
"int" {return KW_INT;}
"long" {return KW_LONG;}
"float" {return KW_FLOAT;}
"bool" {return KW_BOOL;}
"if" {return KW_IF;}
"then" {return KW_THEN;}
"else" {return KW_ELSE;}
"while" {return KW_WHILE;}
"for" {return KW_FOR;}
"read" {return KW_READ;}
"print" {return KW_PRINT;}
"return" {return KW_RETURN;}
"break" {return KW_BREAK;}

"<="	{return OPERATOR_LE;}
">="	{return OPERATOR_GE;}
"=="	{return OPERATOR_EQ;}
"!="	{return OPERATOR_DIF;}


{DIGIT}+ {yylval.symbol = hashInsert(yytext, LIT_INTEGER);return LIT_INTEGER;}
(({DIGIT}+"."{DIGIT}+)|("."{DIGIT}+)|({DIGIT}+".")) {hashInsert(yytext,LIT_FLOAT);return LIT_FLOAT;}
"TRUE"	{yylval.symbol = hashInsert(yytext,LIT_TRUE);return LIT_TRUE;}
"FALSE"	{yylval.symbol = hashInsert(yytext,LIT_FALSE);return LIT_FALSE;}
"'"."'"	{hashInsert(yytext,LIT_CHAR);return LIT_CHAR;}

["]([^"]|\\\")*["]	{yylval.symbol = hashInsert(yytext,LIT_STRING);countLinesInString(yytext,yyleng);return LIT_STRING;}

[\-+*/,;:()\[\]{}*<>=v.~&$#] return (int)yytext[0];

[A-Za-z_][A-Za-z_0-9]* {yylval.symbol = hashInsert(yytext,TK_IDENTIFIER);return TK_IDENTIFIER;}


"\n" {++lineNumber;}
"//".*


"###" BEGIN(COMMENT);
<COMMENT>"###" {BEGIN(INITIAL);}
<COMMENT>"\n" ++lineNumber;
<COMMENT>.

[^ \t]
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



int isRunning(){
  return running;

}
