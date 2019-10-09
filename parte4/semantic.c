#include "semantic.h"
#include "astree.h"

void checkAndSetTypes(AST*node){

    int i;
    if(!node){
      return;
    }
    
    if(node->type == AST_VARDEC){
      if(node->symbol){
      if(node->symbol->type != SYMBOL_IDENTIFIER){
        fprintf(stderr, "Semantic Error: %s already declared\n", node->symbol->text);
      }
        node->symbol->type = SYMBOL_SCALAR;
      }
    }

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

    for(i=0;i<MAX_SONS;++i){
      checkAndSetTypes(node->son[i]);
    }

}

void checkUndeclared(){
  hashCheckUndeclared();
}
