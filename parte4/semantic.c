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
      if(checkFuncParameters(node)==-1){
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
  if(!funcCallNode){
    printf("Ponteiro de chamada de função nula\n");
    return -1;
  }else {
    printf("Funcao de nome %s\n",funDecNode->symbol->text);
    return 1;
  }

}

int checkFuncParameter(AST* funDefParameter, AST* funCallParameter){
  switch(funDefParameter->symbol->datatype){
    case DATATYPE_INT:
    case DATATYPE_FLOAT:
      if(isNodeTypeNumber(funDefParameter)){}
      break;
    case DATATYPE_BOOL:
      if(isNodeTypeBool(funDefParameter)){}
  }
}

int isNodeTypeNumber(AST* node){
  switch(node->type){
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
    case AST_DIV:
    case AST_FUNCALL:
      return 1;//TODO: Teste de tipo de função
      break;
    case AST_SYMBOL:
      if( node->son[i]->symbol->type == SYMBOL_SCALAR &&
            node->son[i]->symbol->datatype != DATATYPE_BOOL){
        return 1;
      }else return 0;
      break;

    case AST_ARRELEMENT:
    case SYMBOL_SCALAR:
      if((node->symbol->datatype==DATATYPE_INT)||(node->symbol->datatype==DATATYPE_FLOAT)||
        (node->symbol->datatype==DATATYPE_SHORT)||(node->symbol->datatype==DATATYPE_LONG)){
        return 1;
      }else return 0;
      break;

    default:
      return 0;

}

int isNodeTypeBool(AST* node){
  switch(node->type){
    
  }
}