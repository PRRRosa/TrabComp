%{
  #include<stdio.h>
  #include<stdlib.h>
%}


%token KW_BYTE
%token KW_INT

%token KW_LONG
%token KW_FLOAT
%token KW_BOOL

%token KW_IF

%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_READ
%token KW_PRINT
%token KW_RETURN
%token KW_BREAK

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR

%token LIT_STRING



%%

programa: programa decl | ;
decl: vardec | fundec 
	;
arrayIndex: LIT_INTEGER| TK_IDENTIFIER
	;
vardec: singleVarDec | arrayDec
	;
singleVar : TK_IDENTIFIER | TK_IDENTIFIER '[' arrayIndex ']'
	;
vartype:KW_BYTE | KW_INT | KW_LONG | KW_FLOAT | KW_BOOL
	;
singleVarDec: vartype TK_IDENTIFIER '=' init ';'
	;
arrayDec: vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listInit ';'| 
		vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ';'
	;
listInit: init| init listInit
	;
init: LIT_INTEGER | LIT_FLOAT | LIT_TRUE | LIT_FALSE | LIT_CHAR
	;
fundec: vartype TK_IDENTIFIER '(' ')' cmd 
	;
cmd: singleVar '=' init | KW_PRINT singleVar | KW_PRINT LIT_STRING | block
	;
block: '{' lcmd '}' ;
lcmd: lcmd cmd ';' | ;
%%
int yyerror(char *msg){
  fprintf(stderr, "Deu erro de sintaxe\n");
  exit(3);
}
