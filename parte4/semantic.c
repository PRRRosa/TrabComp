#include "semantic.h"
#include "astree.h"

int semanticError = 0;
void checkAndSetTypes(AST*node){

    int i;
    if(!node){
      return;
    }

    if(node->type == AST_VARDEC || node->type == AST_FUNDEC || node->type == AST_ARRDEC || node->type == AST_DECPARAM){
      if(node->symbol){
        if(node->symbol->type != SYMBOL_IDENTIFIER){
          fprintf(stderr, "Semantic Error: %s already declared\n", node->symbol->text);
          ++semanticError;
        }
        if (node->type == AST_VARDEC){
          node->symbol->type = SYMBOL_SCALAR;
        }
        if(node->type == AST_FUNDEC){
          node->symbol->type = SYMBOL_FUNCTION;
          node->symbol->funDeclNode = node;
        }
        if(node->type == AST_ARRDEC){
          node->symbol->type = SYMBOL_VECTOR;
        }
        if(node->type == AST_DECPARAM){
          node->symbol->type = SYMBOL_SCALAR;
        }

        if(node->son[0]){
          if(node->son[0]->type == AST_TYPEINT){
            node->symbol->datatype = DATATYPE_INT;
          }
          if(node->son[0]->type == AST_TYPEFLOAT){
            node->symbol->datatype = DATATYPE_FLOAT;
          }
          if(node->son[0]->type == AST_TYPEBYTE){
            node->symbol->datatype = DATATYPE_BYTE;
          }
          if(node->son[0]->type == AST_TYPELONG){
            node->symbol->datatype = DATATYPE_LONG;
          }
        }
      }
    }
/*
    if(node->type == AST_FUNDEC){
      if(node->symbol){
      if(node->symbol->type != SYMBOL_IDENTIFIER){
        fprintf(stderr, "Semantic Error: %s already declared\n", node->symbol->text);
      }
      node->symbol->type = SYMBOL_FUNCTION;
      if(node->son[0]->type == AST_TYPEINT){
        node->symbol->datatype = DATATYPE_INT;
      }
    }
  }

    if(node->type == AST_ARRDEC){
      if(node->symbol){
      if(node->symbol->type != SYMBOL_IDENTIFIER){
        fprintf(stderr, "Semantic Error: %s already declared\n", node->symbol->text);
      }
      node->symbol->type = SYMBOL_VECTOR;
    }
  }
*/
    for(i=0;i<MAX_SONS;++i){
      checkAndSetTypes(node->son[i]);
    }

}
void checkOperands(AST*node){
  int i;
  if(!node){
    return;
  }
  switch(node->type){
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
      for(i=0;i<2;++i){
        if(node->son[i]->type == AST_ADD ||
           node->son[i]->type == AST_SUB ||
           node->son[i]->type == AST_MUL ||
           node->son[i]->type == AST_DIV ||
           (node->son[i]->type == AST_SYMBOL && node->son[i]->symbol->type == SYMBOL_SCALAR &&
            node->son[i]->symbol->datatype != DATATYPE_BOOL) ||
           (node->son[i]->type == AST_SYMBOL && (node->son[i]->symbol->type == SYMBOL_LITINT || node->son[i]->symbol->type == SYMBOL_LITREAL) )
         ){
        ;
        }
        else {
          fprintf(stderr, "Semantic Error: operands not compatible\n");
          ++semanticError;
        }
      }
      break;
    case AST_FUNCALL:
      if(checkFuncParameters(node)==0){
        fprintf(stderr, "Semantic Error: function parameters not compatible\n");
        ++semanticError;
      }
      break;
  }
  for(i=0;i<MAX_SONS;++i){
    checkOperands(node->son[i]);
  }
}
void checkUndeclared(){
  semanticError+=hashCheckUndeclared();
}

int getSemanticError(){
  return semanticError;
}

int checkFuncParameters(AST* funcCallNode){
  AST* funDecNode = funcCallNode->symbol->funDeclNode;
  if(!funcCallNode || !funDecNode){
    printf("Ponteiro de função nula\n");
    return 0;
  }else {
    if(funDecNode->son[1]->type != AST_VARDECLST)
      return 0;
    if(funcCallNode->son[0]->type != AST_ARGLIST)
      return 0;
    printf("Funcao de nome %s\n",funDecNode->symbol->text);
    return checkFuncParameter(funDecNode->son[1],funcCallNode->son[0]);
  }

}

int checkFuncParameter(AST* funDefParameter, AST* funCallParameter){
  if(!funDefParameter){ 
    if(!funCallParameter){
      printf("parâmetros de mesmo tipo\n");
      return 1;
    }else {printf("um param nulo\n");return 0;
      }
  }else if(!funCallParameter){
    printf("um param nulo\n");
    return 0;
  }
  //printf("Comparando param %s com %s\n",funDefParameter->son[0]->symbol->text,funCallParameter->son[0]->symbol->text);
  switch(funDefParameter->son[0]->symbol->datatype){
    case DATATYPE_INT:
    case DATATYPE_FLOAT:
    case DATATYPE_BYTE:
    case DATATYPE_LONG:
      if(isNodeTypeNumber(funCallParameter->son[0])){
        return checkFuncParameter(funDefParameter->son[1], funCallParameter->son[1]);
      }else {printf("parametro de tipo int diferente\n");return 0;}
      break;
    case DATATYPE_BOOL:
      if(isNodeTypeBool(funCallParameter->son[0])){
        return checkFuncParameter(funDefParameter->son[1], funCallParameter->son[1]);
      } else{printf("parametro de tipo bool diferente\n"); return 0;}
      break;
  }
}

int isNodeTypeNumber(AST* node){
  switch(node->type){
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    
      return 1;
      break;
    /*
    case AST_SYMBOL:
      if( node->son[i]->symbol->type == SYMBOL_SCALAR &&
            node->son[i]->symbol->datatype != DATATYPE_BOOL){
        return 1;
      }else return 0;
      break;*/
    case AST_SYMBOL:
    case AST_FUNCALL:
    case AST_ARRELEMENT:
    
      if((node->symbol->datatype==DATATYPE_INT)||(node->symbol->datatype==DATATYPE_FLOAT)||
        (node->symbol->datatype==DATATYPE_BYTE)||(node->symbol->datatype==DATATYPE_LONG)){
        return 1;
      }else return 0;
      break;

    default:
      printf("parametro de tipo não tratado %d\n",node->type);
      return 0;
    }

}

int isNodeTypeBool(AST* node){
  switch(node->type){
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
    case AST_LESS:
    case AST_GRE:
    case AST_OR:
    case AST_SYMBOL:
      return 1;
      break;
    case AST_FUNCALL:
    case AST_ARRELEMENT:
    
      if(node->symbol->datatype==DATATYPE_BOOL){
        return 1;
      }else return 0;
      break;

    default:
    printf("parametro de tipo não tratado\n");
    return 0;
    break;
  }
}