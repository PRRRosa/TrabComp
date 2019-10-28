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
void checkOperands(AST*node, char*currentFunction){
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
        if(isNodeTypeNumber(node->son[i])){
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
    case AST_FUNDEC:
      currentFunction = node->symbol->text;
      break;
    case AST_RETURN:
      if(currentFunction){
        switch(hashFind(currentFunction)->datatype){
          case DATATYPE_INT:
          case DATATYPE_LONG:
          case DATATYPE_FLOAT:
          case DATATYPE_BYTE:
            if(!isNodeTypeNumber(node->son[0])){
              fprintf(stderr, "Semantic Error: return type not compatible\n");
              ++semanticError;
            }
            break;
          case DATATYPE_BOOL:
            if(!isNodeTypeBool(node->son[0])){
              fprintf(stderr, "Semantic Error: return type not compatible\n");
              ++semanticError;
            }
            break;
          default:
            break;
        }
      }
      break;
    case AST_ARRELEMENT:
      if(node->symbol->type != SYMBOL_VECTOR){
        fprintf(stderr, "Semantic Error: Indexation in non array type\n");
        ++semanticError;
      }
      if(node->son[0]){

        if(!isNodeTypeNumber(node->son[0])){
          fprintf(stderr, "Semantic Error: Indexation without number\n");
          ++semanticError;
        }
      }
      break;
    default:
      break;
  }
  for(i=0;i<MAX_SONS;++i){
    checkOperands(node->son[i],currentFunction);
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
    case AST_SYMBOL:
      if(node->symbol->type == SYMBOL_VECTOR){
        return 0;//Se for um vetor, sem ser um elemento específico, não é número
      }//Se não for vetor sozinho, continua:
      if((node->symbol->type==SYMBOL_LITINT)||(node->symbol->type==SYMBOL_LITREAL)||(node->symbol->type==SYMBOL_LITCHAR)){
        return 1;//Se for um literal real, inteiro ou um char, pode ser considerado como um número
      }//Se não for um literal, continua para o teste de variável.
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
      return 1;
      break;

    case AST_SYMBOL:
      if(node->symbol->type == SYMBOL_VECTOR){
        return 0;//Se for um vetor, sem ser um elemento específico, não é booleano
      }//Se não for vetor sozinho, continua:
      if((node->symbol->type==SYMBOL_LITTRUE)||(node->symbol->type==SYMBOL_LITFALSE)){
        return 1;//Se for um literal true ou false, é um valor booleano
      }//Se não for um literal, continua para o teste de variável.
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