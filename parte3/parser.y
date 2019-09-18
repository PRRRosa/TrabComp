%{
  #include<stdio.h>
  #include<stdlib.h>
  int yyerror(char *msg);
  int getLineNumber();
  #include "lex.yy.h"
  #include "hash.h"
  #include "astree.h"
%}

%union{
    HASH_NODE *symbol;
    AST* ast;

}

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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_TRUE
%token<symbol> LIT_FALSE
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%nonassoc IFX
%nonassoc KW_ELSE

%type<ast> expression
%type<ast> init
%left  '*' '/' '+' '-' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>' 'v'

%%

programa:
    programa decl
  |
;
decl:
    vardec
  | fundec
;
vardec:
    singleVarDec
  | arrayDec
;
vartype:
    KW_BYTE
  | KW_INT
  | KW_LONG
  | KW_FLOAT
  | KW_BOOL
;
singleVarDec:
    vartype TK_IDENTIFIER '=' init ';'
;
arrayDec:
    vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listInit ';'
  | vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ';'
;
listInit:
    init
  | init listInit
;
init:
    LIT_INTEGER {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
  | LIT_FLOAT {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
  | LIT_TRUE {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
  | LIT_FALSE {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
  | LIT_CHAR {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
;
fundec:
    vartype TK_IDENTIFIER '(' varDeclFunc ')' cmd
;
cmd:
    assignmentCommand
  | KW_PRINT printString
  | ifCommand
  | KW_READ TK_IDENTIFIER
  | whileCommand
  | funCall
  | forCommand
  | KW_BREAK
  | KW_RETURN expression
  | block
  |
;
assignmentCommand:
    TK_IDENTIFIER '=' expression {astreePrint($3,0);}
  | TK_IDENTIFIER '[' expression ']' '=' expression
;
printString:
    LIT_STRING
  | expression
  | LIT_STRING printString
  | expression printString
;
expression:
    '(' expression ')'
  | expression OPERATOR_LE expression
  | expression OPERATOR_GE expression
  | expression OPERATOR_EQ expression
  | expression OPERATOR_DIF expression
  | expression '<' expression
  | expression '>' expression
  | expression 'v' expression
  | expression '+' expression {$$=astreeCreate(AST_ADD,0,$1,$3,0,0);}
  | expression '-' expression
  | expression '*' expression
  | expression '/' expression
  | init
  | funCall
  | TK_IDENTIFIER {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0);}
  | TK_IDENTIFIER '[' expression ']'
;
whileCommand:
    KW_WHILE '(' expression ')' cmd
;
funCall:
    TK_IDENTIFIER '('argList')'
  | TK_IDENTIFIER '('')'
;
argList:
    expression ',' argList
  | TK_IDENTIFIER ',' argList
  | expression
;
ifCommand:
    KW_IF '(' expression ')' KW_THEN cmd %prec IFX
  | KW_IF '(' expression ')' KW_THEN cmd KW_ELSE cmd
;
forCommand:
    KW_FOR '('TK_IDENTIFIER ':' expression ',' expression ',' expression  ')' cmd
;
varDeclFunc:
    declParam
  | declParam varDeclFuncMeio
  |
;
varDeclFuncMeio:
    ',' declParam varDeclFuncMeio
  | ',' declParam
;
declParam:
    vartype TK_IDENTIFIER
;
block:
    '{' lcmd '}'
;
lcmd:
    cmd
  | cmd lcmdMeio
;
lcmdMeio:
    ';' cmd lcmdMeio
  | ';' cmd
;

%%
int yyerror(char *msg){
  fprintf(stderr, "Deu erro de sintaxe na linha %d\n",getLineNumber());
  exit(3);
}
