#include "tacs.h"
TAC* makeBinOp(int type,TAC* code0, TAC* code1);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2);
TAC* makeWhile(TAC* code0, TAC* code1);
TAC* makeFunction(AST* funcAST,TAC* functionCode);

TAC* tacCreate(int type, HASH_NODE *res,HASH_NODE *op1,HASH_NODE *op2){
  TAC* newtac;
  newtac = (TAC*) calloc(1,sizeof(TAC));
  newtac->type = type;
  newtac->res = res;
  newtac->op1 = op1;
  newtac->op2 = op2;
  newtac->prev = 0;
  newtac->next = 0;
  return newtac;
}
void tacPrintSingle(TAC *tac){
  if(!tac){
    return;
  }
  if(tac->type == TAC_SYMBOL){
    return;
  }
  fprintf(stderr, "TAC( ");
  switch(tac->type){
    case TAC_SYMBOL:
      fprintf(stderr,"TAC_SYMBOL");
      break;
    case TAC_ADD:
      fprintf(stderr,"TAC_ADD");
      break;
    case TAC_SUB:
      fprintf(stderr,"TAC_SUB");
      break;
    case TAC_MUL:
      fprintf(stderr,"TAC_MUL");
      break;
    case TAC_DIV:
      fprintf(stderr,"TAC_DIV");
      break;
    case TAC_MOVE:
      fprintf(stderr,"TAC_MOVE");
      break;
    case TAC_IFZ:
      fprintf(stderr,"TAC_IFZ");
      break;
    case TAC_LABEL:
      fprintf(stderr,"TAC_LABEL");
      break;
    case TAC_PRINT:
      fprintf(stderr,"TAC_PRINT");
      break;
    case TAC_JUMP:
      fprintf(stderr,"TAC_JUMP");
      break;
    case TAC_CALL:
      fprintf(stderr,"TAC_CALL");
      break;
    case TAC_ARG:
      fprintf(stderr,"TAC_ARG");
      break;
    case TAC_BEGINFUN:
      fprintf(stderr,"TAC_BEGINFUN");
      break;
    case TAC_ENDFUN:
      fprintf(stderr,"TAC_ENDFUN");
      break;
    case TAC_LE:
      fprintf(stderr,"TAC_LE");
      break;
    case TAC_GE:
      fprintf(stderr,"TAC_GE");
      break;
    case TAC_EQ:
      fprintf(stderr,"TAC_EQ");
      break;
    case TAC_DIF:
      fprintf(stderr,"TAC_DIF");
      break;
    case TAC_LESS:
      fprintf(stderr,"TAC_LESS");
      break;
    case TAC_GRE:
      fprintf(stderr,"TAC_GRE");
      break;
    case TAC_OR:
      fprintf(stderr,"TAC_OR");
      break;
    default:
      fprintf(stderr,"UNKNOWN");
      break;
  }
  if(tac->res){
    fprintf(stderr," ,%s", tac->res->text);
  }else {
    fprintf(stderr, " ,0");
  }
  if(tac->op1){
    fprintf(stderr," ,%s", tac->op1->text);
  }else {
    fprintf(stderr, " ,0");
  }
  if(tac->op2){
    fprintf(stderr," ,%s", tac->op2->text);
  }else {
    fprintf(stderr, " ,0");
  }
  fprintf(stderr,");\n");
}
void tacPrintBackwards(TAC *tac){
  for(;tac;tac=tac->prev){
    tacPrintSingle(tac);
  }
}
TAC* tacJoin(TAC* l1, TAC* l2){
  TAC* tac;
  if(!l1) return l2;
  if(!l2) return l1;
  for(tac = l2;tac->prev;tac=tac->prev){
    ;
  }
  tac->prev = l1;
  return l2;
}

TAC* generateCode(AST* ast){
  int i;
  TAC *code[MAX_SONS];
  if(!ast) return 0;
  for(i = 0;i<MAX_SONS;++i){
    code[i] = generateCode(ast->son[i]);
  }
  switch(ast->type){
    case AST_SYMBOL:
      return tacCreate(TAC_SYMBOL,ast->symbol,0,0);
      break;
    case AST_ADD:
      //return tacJoin(code[0],tacJoin(code[1],tacCreate(TAC_ADD,makeTemp(),code[0]?code[0]->res:0,code[1]?code[1]->res:0)));
      return makeBinOp(TAC_ADD,code[0],code[1]);
      break;
    case AST_SUB:
      return makeBinOp(TAC_SUB,code[0],code[1]);
      break;
    case AST_MUL:
      return makeBinOp(TAC_MUL,code[0],code[1]);
      break;
    case AST_DIV:
      return makeBinOp(TAC_DIV,code[0],code[1]);
      break;
    case AST_IFCMD:
      if(ast->son[2]){
        return makeIfThenElse(code[0],code[1], code[2]);
      }else return makeIfThen(code[0],code[1]);
      break;
    case AST_ASSIGNCMD:
      return tacJoin(tacJoin(code[0],code[1]),tacCreate(TAC_MOVE,ast->symbol,code[0]?code[0]->res:0,0));
      break;
    case AST_PRINT:
      return tacJoin(code[0],tacCreate(TAC_PRINT,NULL,code[0]?code[0]->res:0,0));
      break;
    case AST_FUNCALL:
      return tacJoin(tacCreate(TAC_CALL,ast->symbol,code[0]?code[0]->res:0,code[1]?code[1]->res:0),code[0]);
      break;
    case AST_ARGLIST:
      return tacJoin(tacJoin(tacCreate(TAC_ARG,code[0]?code[0]->res:0,0,0),code[0]),code[1]);
      break;
    case AST_WHILE:
      return makeWhile(code[0],code[1]);
      break;

    case AST_FUNDEC:
      return makeFunction(ast,code[2]);
      break;

    case AST_LE:
      return makeBinOp(TAC_LE,code[0],code[1]);
      break;
    case AST_GE:
      return makeBinOp(TAC_GE,code[0],code[1]);
      break;
    case AST_EQ:
      return makeBinOp(TAC_EQ,code[0],code[1]);
      break;
    case AST_DIF:
      return makeBinOp(TAC_DIF,code[0],code[1]);
      break;     
    case AST_LESS:
      return makeBinOp(TAC_LESS,code[0],code[1]);
      break;
    case AST_GRE:
      return makeBinOp(TAC_GRE,code[0],code[1]);
      break;
    case AST_OR:
      return makeBinOp(TAC_OR,code[0],code[1]);
      break;
    default:
      return (tacJoin(tacJoin(tacJoin(code[0],code[1]),code[2]),code[3]));
      break;
  }
}
TAC* makeBinOp(int type,TAC* code0, TAC* code1){
  //return tacJoin(tacJoin(code0,code1),tacCreate(type,makeTemp(),code0?code0->res:0,code1?code1->res:0));
  TAC* list=0;
  TAC* novatac = 0;
  novatac = tacCreate(type, makeTemp(),code0?code0->res:0,code1?code1->res:0);
  list = tacJoin(code0,code1);
  novatac->prev = list;
  return novatac;

}
TAC* makeIfThen(TAC* code0, TAC* code1){
  HASH_NODE* label = 0;
  TAC* tacif = 0;
  TAC* taclabel = 0;
  label = makeLabel();
  tacif = tacCreate(TAC_IFZ, label,code0?code0->res:0,0);
  taclabel = tacCreate(TAC_LABEL, label,0,0);

  return tacJoin(tacJoin(tacJoin(code0,tacif),code1),taclabel);
}

TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2){
  HASH_NODE* labelBetween = 0;
  HASH_NODE* labelEnd = 0;
  TAC* tacif = 0;
  TAC* taclabelbetween = 0;
  TAC* taclabelend = 0;
  labelBetween = makeLabel();
  labelEnd = makeLabel();
  tacif = tacCreate(TAC_IFZ, labelBetween,code0?code0->res:0,0);
  taclabelbetween = tacCreate(TAC_LABEL, labelBetween,0,0);
  taclabelend = tacCreate(TAC_LABEL, labelEnd,0,0);

  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(code0,tacif),code1),tacCreate(TAC_JUMP,labelEnd,0,0)),taclabelbetween),code2),taclabelend);
}

TAC* makeCall(TAC* code0,AST* func){
  TAC* callTAC;
  callTAC = tacCreate(TAC_CALL,func->symbol,code0?code0->res:0,0);
  AST* currentArg;
  currentArg = func->son[0];
  while(currentArg){
    callTAC = tacJoin(callTAC,tacCreate(TAC_ARG,currentArg->son[0]->symbol,0,0));
  }
  return callTAC;
}

TAC* makeWhile(TAC* code0, TAC* code1){
  HASH_NODE* labelStart = 0;
  HASH_NODE* labelEnd = 0;
  TAC* tacif = 0;
  TAC* taclabelEnd = 0;
  TAC* taclabelStart = 0;
  TAC* tacJumpStart = 0;
  labelStart = makeLabel();
  labelEnd = makeLabel();
  
  tacif = tacCreate(TAC_IFZ, labelEnd,code0?code0->res:0,0);
  taclabelEnd = tacCreate(TAC_LABEL, labelEnd,0,0);
  taclabelStart = tacCreate(TAC_LABEL, labelStart,0,0);
  tacJumpStart = tacCreate(TAC_JUMP, labelStart,0,0);

  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(taclabelStart,code0),tacif),code1),tacJumpStart),taclabelEnd);
}

TAC* makeFunction(AST* funcAST,TAC* functionCode){
  TAC* tacFuncStart = 0;
  TAC* tacFuncEnd = 0;
  tacFuncStart = tacCreate(TAC_BEGINFUN,funcAST->symbol,0,0);
  tacFuncEnd = tacCreate(TAC_ENDFUN,funcAST->symbol,0,0);
  return tacJoin(tacJoin(tacFuncStart,functionCode),tacFuncEnd);
}