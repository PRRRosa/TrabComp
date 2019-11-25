#include "astree.h"

AST *astreeCreate(int type,HASH_NODE *symbol, AST *s0,AST *s1,AST *s2,AST *s3, int line){
  AST *newNode = 0;
  newNode = (AST*) calloc(1,sizeof(AST));
  newNode->type = type;
  newNode->symbol = symbol;
  newNode->son[0] = s0;
  newNode->son[1] = s1;
  newNode->son[2] = s2;
  newNode->son[3] = s3;
  newNode-> line = line;
return newNode;

}
void astreePrint(AST *node, int level){
  int i;
  if(!node){
    return;
  }
  fprintf(stderr,"\n");
  for(i=0;i<level;++i){
      fprintf(stderr," ");
  }
  fprintf(stderr,"AST(");
  switch(node->type){
    case AST_SYMBOL:
      fprintf(stderr, "AST_SYMBOL,");
      break;
    case AST_ADD:
      fprintf(stderr,"AST_ADD,");
      break;
    case AST_SUB:
      fprintf(stderr,"AST_SUB,");
      break;
    case AST_MUL:
      fprintf(stderr,"AST_MUL,");
      break;
    case AST_DIV:
      fprintf(stderr,"AST_DIV,");
      break;
    case AST_LE:
      fprintf(stderr,"AST_LE,");
      break;
    case AST_GE:
      fprintf(stderr,"AST_GE,");
      break;
    case AST_EQ:
      fprintf(stderr,"AST_EQ,");
      break;
    case AST_DIF:
      fprintf(stderr,"AST_DIF,");
      break;
    case AST_LESS:
      fprintf(stderr,"AST_LESS,");
      break;
    case AST_GRE:
      fprintf(stderr,"AST_GRE,");
      break;
    case AST_OR:
      fprintf(stderr,"AST_OR,");
      break;
    case AST_FUNCALL:
      fprintf(stderr,"AST_FUNCALL,");
      break;
    case AST_ARGLIST:
      fprintf(stderr,"AST_ARGLIST,");
      break;
    case AST_ARRELEMENT:
      fprintf(stderr,"AST_ARRELEMENT,");
      break;
    case AST_ASSIGNCMD:
      fprintf(stderr,"AST_ASSIGNCMD,");
      break;
    case AST_PRINT:
      fprintf(stderr,"AST_PRINT,");
      break;
    case AST_PRINTSTR:
      fprintf(stderr,"AST_PRINTSTR,");
      break;
    case AST_PRINTEXP:
      fprintf(stderr,"AST_PRINTEXP,");
      break;
    case AST_IFCMD:
      fprintf(stderr,"AST_IFCMD,");
      break;
    case AST_READ:
      fprintf(stderr,"AST_READ,");
      break;
    case AST_WHILE:
      fprintf(stderr,"AST_WHILE,");
      break;
    case AST_FOR:
      fprintf(stderr,"AST_FOR,");
      break;
    case AST_BREAK:
      fprintf(stderr,"AST_BREAK,");
      break;
    case AST_RETURN:
      fprintf(stderr,"AST_RETURN,");
      break;
    case AST_BLOCK:
      fprintf(stderr,"AST_BLOCK,");
      break;
    case AST_VARDEC:
      fprintf(stderr,"AST_VARDEC,");
      break;
    case AST_FUNDEC:
      fprintf(stderr,"AST_FUNDEC,");
      break;
    case AST_TYPEBYTE:
      fprintf(stderr,"AST_TYPEBYTE,");
      break;
    case AST_TYPEINT:
      fprintf(stderr,"AST_TYPEINT,");
      break;
    case AST_TYPELONG:
      fprintf(stderr,"AST_TYPELONG,");
      break;
    case AST_TYPEFLOAT:
      fprintf(stderr,"AST_TYPEFLOAT,");
      break;
    case AST_TYPEBOOL:
      fprintf(stderr,"AST_TYPEBOOL,");
      break;
    case AST_LDEC:
      fprintf(stderr,"AST_LDEC,");
      break;
    case AST_ARRDEC:
      fprintf(stderr,"AST_ARRDEC,");
      break;
    case AST_ARRDECINIT:
      fprintf(stderr,"AST_ARRDECINIT,");
      break;
    case AST_LISTINIT:
      fprintf(stderr,"AST_LISTINIT,");
      break;
    case AST_VARDECLST:
      fprintf(stderr,"AST_VARDECLST,");
      break;
    case AST_DECPARAM:
      fprintf(stderr,"AST_DECPARAM,");
      break;
    case AST_LCMD:
      fprintf(stderr,"AST_LCMD,");
      break;
    case BOOL_EXP:
      fprintf(stderr,"BOOL_EXP,");
      break;
    case AST_ARRWRITE:
      fprintf(stderr,"AST_ARRWRITE,");
      break;
    default:
      break;
  }
  if(node->symbol){
    fprintf(stderr,"%s, ", node->symbol->text);
  }else{
    //fprintf(stderr," \n");
  }
  for(i=0;i<MAX_SONS;++i){
    astreePrint(node->son[i], level+1);

  }
  fprintf(stderr,")");
}

void astreeWrite(AST *node,FILE *fileToWrite){
  int i;
  if(!node){
    return;
  }
  /*
  fprintf(fileToWrite,"\n");
  for(i=0;i<level;++i){
      fprintf(fileToWrite," ");
  }*/
  switch(node->type){
    case AST_SYMBOL:
      fprintf(fileToWrite, "%s",node->symbol->text);
      break;
    case AST_ADD:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," + ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_SUB:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," - ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_MUL:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," * ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_DIV:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," / ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_LE:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," <= ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_GE:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," >= ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_EQ:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," == ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_DIF:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," != ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_LESS:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," < ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_GRE:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," > ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_OR:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," v ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_FUNCALL://TK_IDENTIFIER '('argList')' {$$=astreeCreate(AST_FUNCALL,0,$3,0,0,0);}
      fprintf(fileToWrite,"%s (",node->symbol->text);
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,")");
      break;
    case AST_ARGLIST:
      astreeWrite(node->son[0],fileToWrite);
      if(node->son[1]!=0){
        fprintf(fileToWrite,", ");
        astreeWrite(node->son[1],fileToWrite);
      }
      break;
    case AST_ARRELEMENT:
      fprintf(fileToWrite,"%s [",node->symbol->text);
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,"]");
      break;
    case AST_ASSIGNCMD:
//TK_IDENTIFIER '[' expression ']' '=' expression  {$$=astreeCreate(AST_ASSIGNCMD,0, astreeCreate(AST_ARRELEMENT,$1,$3,0,0,0), $6,0,0);}
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," = ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_PRINT://{$$=astreeCreate(AST_PRINT,0,$2,0,0,0);}
      fprintf(fileToWrite,"print ");
      astreeWrite(node->son[0],fileToWrite);
      break;
    case AST_PRINTSTR:
    case AST_PRINTEXP:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_IFCMD:
    //KW_IF '(' expression ')' KW_THEN cmd KW_ELSE cmd  {$$=astreeCreate(AST_IFCMD,0,$3,$6,$8,0);}
      fprintf(fileToWrite,"if (");
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,") then ");
      astreeWrite(node->son[1],fileToWrite);
      if(node->son[2]!=0){
        fprintf(fileToWrite," else ");
        astreeWrite(node->son[2],fileToWrite);
      }

      break;
    case AST_READ://KW_READ TK_IDENTIFIER {$$=astreeCreate(AST_READ,0,astreeCreate(AST_SYMBOL,$2,0,0,0,0),0,0,0);}
      fprintf(fileToWrite,"read ");
      astreeWrite(node->son[0],fileToWrite);
      break;
    case AST_WHILE://KW_WHILE '(' expression ')' cmd {$$=astreeCreate(AST_WHILE,0,$3,$5,0,0);}
      fprintf(fileToWrite,"while (");
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,")");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_FOR:
    //KW_FOR '('TK_IDENTIFIER ':' expression ',' expression ',' expression  ')' cmd {$$=astreeCreate(AST_FOR,$3,$5,$7,$9,$11);}
      fprintf(fileToWrite,"for (%s : ", node->symbol->text);
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,", ");
      astreeWrite(node->son[1],fileToWrite);
      fprintf(fileToWrite,", ");
      astreeWrite(node->son[2],fileToWrite);
      fprintf(fileToWrite,") ");
      astreeWrite(node->son[3],fileToWrite);
      break;
    case AST_BREAK:
      fprintf(fileToWrite,"break");
      break;
    case AST_RETURN:
      fprintf(fileToWrite,"return ");
      astreeWrite(node->son[0],fileToWrite);
      break;
    case AST_BLOCK://'{' lcmd '}'  {$$=astreeCreate(AST_BLOCK,0,$2,0,0,0);}
      fprintf(fileToWrite,"{\n");
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,"}\n");
      break;
    case AST_VARDEC://vartype TK_IDENTIFIER '=' init ';'
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," %s = ", node->symbol->text);
      astreeWrite(node->son[1],fileToWrite);
      fprintf(fileToWrite," ;\n");
      break;
    case AST_ARRDEC://vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listInit ';' vartype,nodo para int,List
      //{$$=astreeCreate(AST_ARRDEC,$2,$1, astreeCreate(AST_SYMBOL,$4,0,0,0,0),$7,0);}
      astreeWrite(node->son[0],fileToWrite);//tipo da var.
      fprintf(fileToWrite," %s", node->symbol->text);// Nome da variável sendo declarada

      fprintf(fileToWrite,"[");
      astreeWrite(node->son[1],fileToWrite);//índicenode->symbol->text

      fprintf(fileToWrite,"]");


      fprintf(fileToWrite,";\n");
      break;
    case AST_ARRDECINIT://vartype TK_IDENTIFIER '[' LIT_INTEGER ']' ':' listInit ';' vartype,nodo para int,List
      //{$$=astreeCreate(AST_ARRDEC,$2,$1, astreeCreate(AST_SYMBOL,$4,0,0,0,0),$7,0);}
      astreeWrite(node->son[0],fileToWrite);//tipo da var.
      fprintf(fileToWrite," %s", node->symbol->text);// Nome da variável sendo declarada

      fprintf(fileToWrite,"[");
      astreeWrite(node->son[1],fileToWrite);//índicenode->symbol->text

      fprintf(fileToWrite,"]");
      // Se a declaração tiver uma inicialização
        fprintf(fileToWrite," : ");
        astreeWrite(node->son[2],fileToWrite);//imprime a lista da inicialização do array


      fprintf(fileToWrite,";\n");
      break;
    case AST_FUNDEC://vartype TK_IDENTIFIER '(' varDeclFunc ')' cmd {$$=astreeCreate(AST_FUNDEC,$2,$1,$4,$6,0);}
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," %s (", node->symbol->text);
      astreeWrite(node->son[1],fileToWrite);
      fprintf(fileToWrite,") ");
      astreeWrite(node->son[2],fileToWrite);
      fprintf(fileToWrite,"\n");
      break;
    case AST_TYPEBYTE:
      fprintf(fileToWrite,"byte");
      break;
    case AST_TYPEINT:
      fprintf(fileToWrite,"int");
      break;
    case AST_TYPELONG:
      fprintf(fileToWrite,"long");
      break;
    case AST_TYPEFLOAT:
      fprintf(fileToWrite,"float");
      break;
    case AST_TYPEBOOL:
      fprintf(fileToWrite,"bool");
      break;
    case AST_LDEC:
      astreeWrite(node->son[0],fileToWrite);
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_LISTINIT://init listInit   {$$=astreeCreate(AST_LISTINIT,0,$1,$2,0,0);}
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," ");
      astreeWrite(node->son[1],fileToWrite);
      break;
    case AST_VARDECLST:
      astreeWrite(node->son[0],fileToWrite);
      if(node->son[1]!=0){
        fprintf(fileToWrite,", ");
        astreeWrite(node->son[1],fileToWrite);
      }
      break;
    case AST_DECPARAM:
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite," %s", node->symbol->text);
      break;
    case AST_LCMD://cmd lcmdMeio  {$$=astreeCreate(AST_LCMD,0,$1,$2,0,0);}

      astreeWrite(node->son[0],fileToWrite);
      if(node->son[1]!=0){
        fprintf(fileToWrite,";\n");
        astreeWrite(node->son[1],fileToWrite);
      }
      break;

    case BOOL_EXP:
      fprintf(fileToWrite,"(");
      astreeWrite(node->son[0],fileToWrite);
      fprintf(fileToWrite,")");
      break;

    default:
      break;
  }
  /*
  if(node->symbol){
    fprintf(fileToWrite,"%s, ", node->symbol->text);
  }else{
    //fprintf(fileToWrite," \n");
  }
  for(i=0;i<MAX_SONS;++i){
    astreePrint(node->son[i], level+1);

  }*/
}

//END
