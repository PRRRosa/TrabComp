#include "tacs.h"
TAC* makeBinOp(int type,TAC* code0, TAC* code1);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2);
TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* labelLoopEnd);
TAC* makeFunction(AST* funcAST,TAC* functionCode);
TAC* makeFor(HASH_NODE* loopVar,TAC* tacInitOp, TAC* tacIfOp, TAC* tacLoopOp, TAC* codigoFor, HASH_NODE* labelLoopEnd);
TAC* makeAssign(HASH_NODE* assignVar, TAC* code1);
TAC* makePrint(AST* node, HASH_NODE* labelLoopEnd);
TAC* makeBreak(HASH_NODE* labelLoopEnd);
TAC* makeCall(AST* funcCall);
TAC* makeArrayWrite(HASH_NODE* vectorName,TAC* vectorIndexCode,TAC* assignCode);

TAC* tacCreate(int type, HASH_NODE *res,HASH_NODE *op1,HASH_NODE *op2, int argNumber){
  TAC* newtac;
  newtac = (TAC*) calloc(1,sizeof(TAC));
  newtac->type = type;
  newtac->res = res;
  newtac->op1 = op1;
  newtac->op2 = op2;
  newtac->prev = 0;
  newtac->next = 0;
  newtac->argNumber = argNumber;
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
    case TAC_ARREF:
      fprintf(stderr,"TAC_ARREF");
      break;
    case TAC_RET:
      fprintf(stderr, "TAC_RET");
      break;
    case TAC_READ:
      fprintf(stderr, "TAC_READ");
      break;
    case TAC_VAR:
      fprintf(stderr, "TAC_VAR");
      break;
    case TAC_VEC:
      fprintf(stderr, "TAC_VEC");
      break;
    case TAC_VECINIT:
      fprintf(stderr, "TAC_VECINIT");
      break;
    case TAC_ARRWRITE:
      fprintf(stderr, "TAC_ARRWRITE");
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
  if(tac->argNumber){
    fprintf(stderr," ,%d", tac->argNumber);
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

TAC* generateCode(AST* ast,HASH_NODE* labelLoopEnd){
  int i;
  TAC *code[MAX_SONS];
  if(!ast) return 0;
  if((ast->type == AST_FOR)||(ast->type == AST_WHILE)){
    labelLoopEnd = makeLabel();
  }
  for(i = 0;i<MAX_SONS;++i){
    code[i] = generateCode(ast->son[i],labelLoopEnd);
  }
  switch(ast->type){
    case AST_SYMBOL:
      return tacCreate(TAC_SYMBOL,ast->symbol,0,0,0);
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
      return makeAssign(code[0]?code[0]->res:0,code[1]);
      //return tacJoin(tacJoin(code[0],code[1]),tacCreate(TAC_MOVE,code[1]?code[1]->res:0,code[0]?code[0]->res:0,0));
      break;
    case AST_PRINT:
      return makePrint(ast, labelLoopEnd);
      break;
    case AST_FUNCALL:
      //return tacJoin(code[0],tacCreate(TAC_CALL,ast->symbol,code[0]?code[0]->res:0,code[1]?code[1]->res:0,0));
      return makeCall(ast);
      break;
    case AST_ARGLIST:
      return tacJoin(tacJoin(tacCreate(TAC_ARG,code[0]?code[0]->res:0,0,0,0),code[0]),code[1]);
      break;
    case AST_WHILE:
      return makeWhile(code[0],code[1],labelLoopEnd);
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
    case AST_ARRELEMENT:
      return tacJoin(code[0], tacCreate(TAC_ARREF,makeTemp(),ast->symbol,code[0]?code[0]->res:0,0));
      break;
    case AST_ARRWRITE:
      return makeArrayWrite(ast->symbol,code[0],code[1]);
      break;
    case AST_RETURN:
      return tacJoin(code[0],tacCreate(TAC_RET,code[0]?code[0]->res:0,0,0,0));
      break;
    case AST_READ:
      return tacCreate(TAC_READ,code[0]?code[0]->res:0,0,0,0);
      break;
    case AST_VARDEC:
      return tacJoin(tacCreate(TAC_SYMBOL,ast->symbol,0,0,0),tacJoin(code[1],tacCreate(TAC_VAR,ast->symbol,code[1]->res,0,0)));
      break;
    case AST_ARRDEC:
      return tacJoin(tacCreate(TAC_VEC,ast->symbol,ast->son[0]->symbol,0,0),code[0]);
      break;
    case AST_ARRDECINIT:
      return tacJoin(tacCreate(TAC_VECINIT,ast->symbol,ast->son[0]->symbol,ast->son[2]->symbol,0),code[0]);
      break;
    case AST_FOR:
      return makeFor(ast->symbol,code[0],code[1],code[2],code[3],labelLoopEnd);
      break;
    case AST_BREAK:
      return makeBreak(labelLoopEnd);
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
  novatac = tacCreate(type, makeTemp(),code0?code0->res:0,code1?code1->res:0,0);
  list = tacJoin(code0,code1);
  novatac->prev = list;
  return novatac;

}
TAC* makeIfThen(TAC* code0, TAC* code1){
  HASH_NODE* label = 0;
  TAC* tacif = 0;
  TAC* taclabel = 0;
  label = makeLabel();
  tacif = tacCreate(TAC_IFZ, label,code0?code0->res:0,0,0);
  taclabel = tacCreate(TAC_LABEL, label,0,0,0);

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
  tacif = tacCreate(TAC_IFZ, labelBetween,code0?code0->res:0,0,0);
  taclabelbetween = tacCreate(TAC_LABEL, labelBetween,0,0,0);
  taclabelend = tacCreate(TAC_LABEL, labelEnd,0,0,0);

  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(code0,tacif),code1),tacCreate(TAC_JUMP,labelEnd,0,0,0)),taclabelbetween),code2),taclabelend);
}

TAC* makeCall(AST* funcCall){
  /*TAC* callTAC;
  callTAC = tacCreate(TAC_CALL,func->symbol,code0?code0->res:0,0);
  AST* currentArg;
  currentArg = func->son[0];
  while(currentArg){
    callTAC = tacJoin(callTAC,tacCreate(TAC_ARG,currentArg->son[0]->symbol,0,0));
  }
  return callTAC;*/
  TAC* tacCall = 0;
	TAC* params = 0;
	AST* buff = 0;
	TAC* tacBuff = 0;
	TAC* tacArg = 0;
	int i = 1;
	HASH_NODE* func_name = funcCall->symbol;
	for(buff = funcCall->son[0]; buff; buff = buff->son[1]){
		tacBuff = generateCode(buff->son[0],0); //expr or a symbol... tacGenerate can process
		tacArg = tacCreate(TAC_ARG, 0, tacBuff->res,func_name,i);
		params = tacJoin(tacJoin(params,tacBuff),tacArg);
		i++;
	}

	HASH_NODE* tempCall = makeTemp();
	tacCall = tacCreate(TAC_CALL, tempCall, funcCall->symbol,0,0);
	return tacJoin(params,tacCall);
}

TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* labelLoopEnd){
  HASH_NODE* labelStart = 0;
  HASH_NODE* labelEnd = 0;
  TAC* tacif = 0;
  TAC* taclabelEnd = 0;
  TAC* taclabelStart = 0;
  TAC* tacJumpStart = 0;
  labelStart = makeLabel();
  labelEnd = labelLoopEnd;

  tacif = tacCreate(TAC_IFZ, labelEnd,code0?code0->res:0,0,0);
  taclabelEnd = tacCreate(TAC_LABEL, labelEnd,0,0,0);
  taclabelStart = tacCreate(TAC_LABEL, labelStart,0,0,0);
  tacJumpStart = tacCreate(TAC_JUMP, labelStart,0,0,0);

  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(taclabelStart,code0),tacif),code1),tacJumpStart),taclabelEnd);
}

TAC* makeFunction(AST* funcAST,TAC* functionCode){
  TAC* tacFuncStart = 0;
  TAC* tacFuncEnd = 0;
  tacFuncStart = tacCreate(TAC_BEGINFUN,funcAST->symbol,0,0,0);
  tacFuncEnd = tacCreate(TAC_ENDFUN,funcAST->symbol,0,0,0);
  return tacJoin(tacJoin(tacFuncStart,functionCode),tacFuncEnd);
}

/*tacInitOp: Operação para inicializar variável para o for
  tacIfOp:   Operação para testar a variável durante o for
  tacLoopOp: Operação para incrementar (ou decrementar) a variável do for
  codigoFor: Código dentro do for
*/
TAC* makeFor(HASH_NODE* loopVar,TAC* tacInitOp, TAC* tacIfOp, TAC* tacLoopOp, TAC* codigoFor, HASH_NODE* labelLoopEnd){
  HASH_NODE* labelStart = 0;
  HASH_NODE* labelEnd = 0;
  TAC* tacifComparison = 0;
  TAC* tacif = 0;
  TAC* taclabelEnd = 0;
  TAC* taclabelStart = 0;
  TAC* tacJumpStart = 0;
  TAC* tacInitAssign = 0;
  labelStart = makeLabel();
  labelEnd = labelLoopEnd;

  tacInitAssign = makeAssign(loopVar,tacInitOp);

  /*Adiciona ao final da operação do valor a ser testado com a variável do loop
    a comparação entre esse valor e a variável do loop
  */
  tacifComparison = tacCreate(TAC_LESS, makeTemp(),loopVar,tacIfOp?tacIfOp->res:0,0);
  tacif = tacJoin(tacifComparison,tacCreate(TAC_IFZ, labelEnd,tacifComparison?tacifComparison->res:0,0,0));

  taclabelEnd = tacCreate(TAC_LABEL, labelEnd,0,0,0);
  taclabelStart = tacCreate(TAC_LABEL, labelStart,0,0,0);
  tacJumpStart = tacCreate(TAC_JUMP, labelStart,0,0,0);

  /*Adiciona ao final da operação do loop uma soma do elemento final à variável do loop e
    uma operação para mover esse valor de volta à variável do loop
  */
  HASH_NODE* tacLoopOpResult = makeTemp();
  tacLoopOp = tacJoin(tacJoin(tacLoopOp, tacCreate(TAC_ADD, tacLoopOpResult,loopVar,tacLoopOp?tacLoopOp->res:0,0)),
      tacCreate(TAC_MOVE,loopVar,tacLoopOpResult,0,0));

  return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacInitAssign,taclabelStart),tacIfOp),tacif),codigoFor),tacLoopOp),tacJumpStart),taclabelEnd);
}

TAC* makeAssign(HASH_NODE* assignVar, TAC* code1){
  return tacJoin(code1,tacCreate(TAC_MOVE,assignVar,code1?code1->res:0,0,0));
}

TAC* makePrint(AST* node,HASH_NODE* labelLoopEnd){
  AST* buff = node->son[0];
	TAC* result = 0;
	TAC* tacBuff = 0;
	TAC* tacPrint = 0;
	while(buff){
		if(buff->symbol){
			tacBuff = tacCreate(TAC_SYMBOL,buff->symbol, 0, 0,0);
			buff = buff->son[0];
		}else{
			tacBuff = generateCode(buff->son[0],labelLoopEnd);
			buff = buff->son[1];
		}
		tacPrint = tacCreate(TAC_PRINT,tacBuff ? tacBuff->res: 0,0,0,0);
		result = tacJoin(tacJoin(result,tacBuff),tacPrint);
	}
	return result;
}

TAC* makeBreak(HASH_NODE* labelLoopEnd){
  if(labelLoopEnd)
    return tacCreate(TAC_JUMP,labelLoopEnd,0,0,0);
  else return 0;
}

TAC* makeArrayWrite(HASH_NODE* vectorName,TAC* vectorIndexCode,TAC* assignCode){
  TAC* writeTAC = tacCreate(TAC_ARRWRITE,vectorName,vectorIndexCode?vectorIndexCode->res:0,assignCode?assignCode->res:0,0);
  return tacJoin(vectorIndexCode,tacJoin(assignCode,writeTAC));
}
