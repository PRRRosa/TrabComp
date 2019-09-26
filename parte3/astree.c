#include "astree.h"

AST *astreeCreate(int type,HASH_NODE *symbol, AST *s0,AST *s1,AST *s2,AST *s3){
  AST *newNode = 0;
  newNode = (AST*) calloc(1,sizeof(AST));
  newNode->type = type;
  newNode->symbol = symbol;
  newNode->son[0] = s0;
  newNode->son[1] = s1;
  newNode->son[2] = s2;
  newNode->son[3] = s3;
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
    case AST_UNIMPL:
      fprintf(stderr,"AST_UNIMPL,");
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
      fprintf(fileToWrite,"AST_SUB,");
      break;
    case AST_MUL:
      fprintf(fileToWrite,"AST_MUL,");
      break;
    case AST_DIV:
      fprintf(fileToWrite,"AST_DIV,");
      break;
    case AST_LE:
      fprintf(fileToWrite,"AST_LE,");
      break;
    case AST_GE:
      fprintf(fileToWrite,"AST_GE,");
      break;
    case AST_EQ:
      fprintf(fileToWrite,"AST_EQ,");
      break;
    case AST_DIF:
      fprintf(fileToWrite,"AST_DIF,");
      break;
    case AST_LESS:
      fprintf(fileToWrite,"AST_LESS,");
      break;
    case AST_GRE:
      fprintf(fileToWrite,"AST_GRE,");
      break;
    case AST_OR:
      fprintf(fileToWrite,"AST_OR,");
      break;
    case AST_FUNCALL:
      fprintf(fileToWrite,"AST_FUNCALL,");
      break;
    case AST_ARGLIST:
      fprintf(fileToWrite,"AST_ARGLIST,");
      break;
    case AST_ARRELEMENT:
      fprintf(fileToWrite,"AST_ARRELEMENT,");
      break;
    case AST_ASSIGNCMD:
      fprintf(fileToWrite,"AST_ASSIGNCMD,");
      break;
    case AST_PRINT:
      fprintf(fileToWrite,"AST_PRINT,");
      break;
    case AST_PRINTSTR:
      fprintf(fileToWrite,"AST_PRINTSTR,");
      break;
    case AST_UNIMPL:
      fprintf(fileToWrite,"AST_UNIMPL,");
      break;
    case AST_IFCMD:
      fprintf(fileToWrite,"AST_IFCMD,");
      break;
    case AST_READ:
      fprintf(fileToWrite,"AST_READ,");
      break;
    case AST_WHILE:
      fprintf(fileToWrite,"AST_WHILE,");
      break;
    case AST_FOR:
      fprintf(fileToWrite,"AST_FOR,");
      break;
    case AST_BREAK:
      fprintf(fileToWrite,"AST_BREAK,");
      break;
    case AST_RETURN:
      fprintf(fileToWrite,"AST_RETURN,");
      break;
    case AST_BLOCK:
      fprintf(fileToWrite,"AST_BLOCK,");
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
      if(node->son[2]!=0){// Se a declaração tiver uma inicialização
        fprintf(fileToWrite," : ");
        astreeWrite(node->son[2],fileToWrite);//imprime a lista da inicialização do array
      }
      
      fprintf(fileToWrite,";\n");
      break;
    case AST_FUNDEC:
      fprintf(fileToWrite,"AST_FUNDEC,");
      break;
    case AST_TYPEBYTE:
      fprintf(fileToWrite,"AST_TYPEBYTE,");
      break;
    case AST_TYPEINT:
      fprintf(fileToWrite,"AST_TYPEINT,");
      break;
    case AST_TYPELONG:
      fprintf(fileToWrite,"AST_TYPELONG,");
      break;
    case AST_TYPEFLOAT:
      fprintf(fileToWrite,"AST_TYPEFLOAT,");
      break;
    case AST_TYPEBOOL:
      fprintf(fileToWrite,"AST_TYPEBOOL,");
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
      fprintf(fileToWrite,"AST_VARDECLST,");
      break;
    case AST_DECPARAM:
      fprintf(fileToWrite,"AST_DECPARAM,");
      break;
    case AST_LCMD:
      fprintf(fileToWrite,"AST_LCMD,");
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
