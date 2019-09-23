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
//END
