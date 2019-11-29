%{
  #include<stdio.h>
  #include<stdlib.h>
  int yyerror(char *msg);
  int getLineNumber();
  #include "lex.yy.h"
  #include "hash.h"
  #include "astree.h"
  #include "main.h"
  #include "semantic.h"
  #include "tacs.h"
  TAC* tac = 0;
  FILE* asmbly;
%}

%union{
    HASH_NODE *symbol;
    AST* ast;

}

%token<symbol>  KW_BYTE
%token<symbol>  KW_INT
%token<symbol>  KW_LONG
%token<symbol>  KW_FLOAT
%token<symbol>  KW_BOOL
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
%type<ast> funCall
%type<ast> argList
%type<ast> cmd
%type<ast> assignmentCommand
%type<ast> printString
%type<ast> ifCommand
%type<ast> whileCommand
%type<ast> forCommand
%type<ast> lcmd
%type<ast> lcmdMeio
%type<ast> block
%type<ast> programa
%type<ast> vartype
%type<ast> singleVarDec
%type<ast> fundec
%type<ast> decl
%type<ast> varDeclFunc
%type<ast> vardec
%type<ast> arrayDec
%type<ast> listInit
%type<ast> declParam
%type<ast> varDeclFuncMeio

%left 'v'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left  '+' '-'
%left  '*' '/'

%%

begin:
  programa {astreeWrite($1,yyout);astreePrint($1,0);printf("\n");checkAndSetTypes($1);checkOperands($1,NULL);fprintf(stderr,"%d semantic errors\n",getSemanticError());if(getSemanticError() > 0)exit(4);tacPrintBackwards(generateCode($1,0));tac = generateCode($1,0);asmbly = fopen("etapa6.s","w");writeFixed(tac,asmbly);generateASM(tac,asmbly);}
;

programa:
    programa decl {$$=astreeCreate(AST_LDEC,0,$1,$2,0,0,getLineNumber());}
  | {$$=0;}
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
    KW_BYTE {$$=astreeCreate(AST_TYPEBYTE,0,0,0,0,0,getLineNumber());}
  | KW_INT {$$=astreeCreate(AST_TYPEINT,0,0,0,0,0,getLineNumber());}
  | KW_LONG {$$=astreeCreate(AST_TYPELONG,0,0,0,0,0,getLineNumber());}
  | KW_FLOAT {$$=astreeCreate(AST_TYPEFLOAT,0,0,0,0,0,getLineNumber());}
  | KW_BOOL {$$=astreeCreate(AST_TYPEBOOL,0,0,0,0,0,getLineNumber());}
;
singleVarDec:
    vartype TK_IDENTIFIER '=' init ';' {$$=astreeCreate(AST_VARDEC,$2,$1,$4,0,0,getLineNumber());}
;
arrayDec:
    vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listInit ';'  {$$=astreeCreate(AST_ARRDECINIT,$2,$1,astreeCreate(AST_SYMBOL,$4,0,0,0,0,getLineNumber()),$7,0,getLineNumber());}
  | vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ';' {$$=astreeCreate(AST_ARRDEC,$2,$1,astreeCreate(AST_SYMBOL,$4,0,0,0,0,getLineNumber()),0,0,getLineNumber());}
;
listInit:
    init  {$$=astreeCreate(AST_LISTINIT,0,$1,0,0,0,getLineNumber());}
  | init listInit   {$$=astreeCreate(AST_LISTINIT,0,$1,$2,0,0,getLineNumber());}
;
init:
    LIT_INTEGER {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
  | LIT_FLOAT {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
  | LIT_TRUE {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
  | LIT_FALSE {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
  | LIT_CHAR {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
;
fundec:
    vartype TK_IDENTIFIER '(' varDeclFunc ')' cmd {$$=astreeCreate(AST_FUNDEC,$2,$1,$4,$6,0,getLineNumber());}
;
cmd:
    assignmentCommand
  | KW_PRINT printString  {$$=astreeCreate(AST_PRINT,0,$2,0,0,0,getLineNumber());}
  | ifCommand
  | KW_READ TK_IDENTIFIER {$$=astreeCreate(AST_READ,0,astreeCreate(AST_SYMBOL,$2,0,0,0,0,getLineNumber()),0,0,0,getLineNumber());}
  | whileCommand
  | funCall
  | forCommand
  | KW_BREAK {$$=astreeCreate(AST_BREAK,0,0,0,0,0,getLineNumber());}
  | KW_RETURN expression {$$=astreeCreate(AST_RETURN,0,$2,0,0,0,getLineNumber());}
  | block
  | {$$=0;}
;
assignmentCommand:
    TK_IDENTIFIER '=' expression {$$=astreeCreate(AST_ASSIGNCMD,0, astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber()), $3,0,0,getLineNumber());}
  | TK_IDENTIFIER '[' expression ']' '=' expression  {$$=astreeCreate(AST_ARRWRITE,$1, $3, $6,0,0,getLineNumber());}
;
printString:
    LIT_STRING  {$$=astreeCreate(AST_PRINTSTR,0,astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber()),0,0,0,getLineNumber());}
  | expression  {$$=astreeCreate(AST_PRINTEXP,0,$1,0,0,0,getLineNumber());}
  | LIT_STRING printString  {$$=astreeCreate(AST_PRINTSTR,0,astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber()),$2,0,0,getLineNumber());}
  | expression printString  {$$=astreeCreate(AST_PRINTEXP,0,$1,$2,0,0,getLineNumber());}
;
expression:
    '(' expression ')'  {$$=$2;}
  | expression OPERATOR_LE expression {$$=astreeCreate(AST_LE,0,$1,$3,0,0,getLineNumber());}
  | expression OPERATOR_GE expression {$$=astreeCreate(AST_GE,0,$1,$3,0,0,getLineNumber());}
  | expression OPERATOR_EQ expression {$$=astreeCreate(AST_EQ,0,$1,$3,0,0,getLineNumber());}
  | expression OPERATOR_DIF expression {$$=astreeCreate(AST_DIF,0,$1,$3,0,0,getLineNumber());}
  | expression '<' expression {$$=astreeCreate(AST_LESS,0,$1,$3,0,0,getLineNumber());}
  | expression '>' expression {$$=astreeCreate(AST_GRE,0,$1,$3,0,0,getLineNumber());}
  | expression 'v' expression {$$=astreeCreate(AST_OR,0,$1,$3,0,0,getLineNumber());}
  | expression '+' expression {$$=astreeCreate(AST_ADD,0,$1,$3,0,0,getLineNumber());}
  | expression '-' expression {$$=astreeCreate(AST_SUB,0,$1,$3,0,0,getLineNumber());}
  | expression '*' expression {$$=astreeCreate(AST_MUL,0,$1,$3,0,0,getLineNumber());}
  | expression '/' expression {$$=astreeCreate(AST_DIV,0,$1,$3,0,0,getLineNumber());}
  | init
  | funCall
  | TK_IDENTIFIER {$$=astreeCreate(AST_SYMBOL,$1,0,0,0,0,getLineNumber());}
  | TK_IDENTIFIER '[' expression ']'  {$$=astreeCreate(AST_ARRELEMENT,$1,$3,0,0,0,getLineNumber());}
;
whileCommand:
    KW_WHILE '(' expression ')' cmd {$$=astreeCreate(AST_WHILE,0,$3,$5,0,0,getLineNumber());}
;
funCall:
    TK_IDENTIFIER '('argList')' {$$=astreeCreate(AST_FUNCALL,$1,$3,0,0,0,getLineNumber());}
  | TK_IDENTIFIER '('')'  {$$=astreeCreate(AST_FUNCALL,$1,0,0,0,0,getLineNumber());}
;
argList:
    expression ',' argList  {$$=astreeCreate(AST_ARGLIST,0,$1,$3,0,0,getLineNumber());}
  | expression  {$$=astreeCreate(AST_ARGLIST,0,$1,0,0,0,getLineNumber());}
;
ifCommand:
    KW_IF '(' expression ')' KW_THEN cmd %prec IFX  {$$=astreeCreate(AST_IFCMD,0,$3,$6,0,0,getLineNumber());}
  | KW_IF '(' expression ')' KW_THEN cmd KW_ELSE cmd  {$$=astreeCreate(AST_IFCMD,0,$3,$6,$8,0,getLineNumber());}
;
forCommand:
    KW_FOR '('TK_IDENTIFIER ':' expression ',' expression ',' expression  ')' cmd {$$=astreeCreate(AST_FOR,$3,$5,$7,$9,$11,getLineNumber());}
;
varDeclFunc:
    declParam {$$=astreeCreate(AST_VARDECLST,0,$1,0,0,0,getLineNumber());}
  | declParam varDeclFuncMeio {$$=astreeCreate(AST_VARDECLST,0,$1,$2,0,0,getLineNumber());}
  | {$$=0;}
;
varDeclFuncMeio:
    ',' declParam varDeclFuncMeio {$$=astreeCreate(AST_VARDECLST,0,$2,$3,0,0,getLineNumber());}
  | ',' declParam {$$=astreeCreate(AST_VARDECLST,0,$2,0,0,0,getLineNumber());}
;
declParam:
    vartype TK_IDENTIFIER {$$=astreeCreate(AST_DECPARAM,$2,$1,0,0,0,getLineNumber());}
;
block:
    '{' lcmd '}'  {$$=astreeCreate(AST_BLOCK,0,$2,0,0,0,getLineNumber());}
;
lcmd:
    cmd {$$=astreeCreate(AST_LCMD,0,$1,0,0,0,getLineNumber());}
  | cmd lcmdMeio  {$$=astreeCreate(AST_LCMD,0,$1,$2,0,0,getLineNumber());}
;
lcmdMeio:
    ';' cmd lcmdMeio  {$$=astreeCreate(AST_LCMD,0,$2,$3,0,0,getLineNumber());}
  | ';' cmd {$$=astreeCreate(AST_LCMD,0,$2,0,0,0,getLineNumber());}
;

%%
int yyerror(char *msg){
  fprintf(stderr, "Deu erro de sintaxe na linha %d\n",getLineNumber());
  exit(3);
}
