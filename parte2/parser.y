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

programa: programa decl | cmd|;
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
fundec: vartype TK_IDENTIFIER '(' varDeclFunc ')' cmd 
	;
cmd: singleVar '=' init | singleVar '=' funCall | KW_PRINT printString | ifCommand | KW_READ singleVar | whileCommand  | funCall | forCommand | KW_BREAK | block
	;
printString: LIT_STRING | singleVar| LIT_STRING printString | singleVar printString
	;
ifCommand: KW_IF boolExp KW_THEN cmd KW_ELSE cmd
	;
boolExp: singleBool operator singleBool |  boolExp operator singleBool | singleBool operator boolExp | boolExp operator boolExp
	;
singleBool: init|TK_IDENTIFIER
	;
operator: OPERATOR_LE | OPERATOR_GE | OPERATOR_EQ | OPERATOR_DIF | '<' | '>' | 'v' | '+' | '-' | '*' | '/' 
	;
whileCommand: KW_WHILE '(' boolExp ')' '{' lcmd '}'
	;
funCall: TK_IDENTIFIER '('argList')'|TK_IDENTIFIER '('')'
	;
argList: init ',' argList| TK_IDENTIFIER ',' argList| init | TK_IDENTIFIER
	;
ifCommand: KW_IF '(' boolExp ')' KW_THEN cmd
	;
forCommand: KW_FOR '('TK_IDENTIFIER ':' LIT_INTEGER ',' LIT_INTEGER ',' LIT_INTEGER  ')' cmd |
			KW_FOR '('TK_IDENTIFIER ':' LIT_INTEGER ',' LIT_INTEGER ',' LIT_INTEGER  ')' cmd KW_ELSE cmd
	;
varDeclFunc: vartype TK_IDENTIFIER| vartype TK_IDENTIFIER ',' varDeclFunc|
	;

block: '{' lcmd '}' ;
lcmd: lcmd cmd ';' | ;
%%
int yyerror(char *msg){
  fprintf(stderr, "Deu erro de sintaxe\n");
  exit(3);
}
