#include "tacs.h"
TAC* makeBinOp(int type,TAC* code0, TAC* code1);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2);
TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* labelLoopEnd);
TAC* makeFunction(AST* funcAST,TAC* functionParams,TAC* functionCode);
TAC* makeFor(HASH_NODE* loopVar,TAC* tacInitOp, TAC* tacIfOp, TAC* tacLoopOp, TAC* codigoFor, HASH_NODE* labelLoopEnd);
TAC* makeAssign(HASH_NODE* assignVar, TAC* code1);
TAC* makePrint(AST* node, HASH_NODE* labelLoopEnd);
TAC* makeBreak(HASH_NODE* labelLoopEnd);
TAC* makeCall(AST* funcCall);
TAC* makeArrayWrite(HASH_NODE* vectorName,TAC* vectorIndexCode,TAC* assignCode);
void writeVar(TAC* tac, FILE* fout);
void writeMove(char* source, char* target, int sourceDatatype, int targetDatatype, FILE* fout);
void writeBinOp(TAC* operation, FILE* fout);

int printLabelCount = 4;

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
    case TAC_PRINTSTR:
      fprintf(stderr,"TAC_PRINTSTR");
      break;
    case TAC_PRINTEXP:
      fprintf(stderr,"TAC_PRINTEXP");
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
    case TAC_VECINITLIST:
      fprintf(stderr, "TAC_VECINITLIST");
      break;
    case TAC_ARRWRITE:
      fprintf(stderr, "TAC_ARRWRITE");
      break;
    case TAC_ARGDEC:
      fprintf(stderr, "TAC_ARGDEC");
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
  if((ast->type != AST_PRINT)&&(ast->type != AST_FUNCALL)){
    for(i = 0;i<MAX_SONS;++i){
      code[i] = generateCode(ast->son[i],labelLoopEnd);
    }
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
      return makeFunction(ast,code[1],code[2]);
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
      return tacJoin(tacCreate(TAC_VEC,ast->symbol,ast->son[0]->symbol,ast->son[1]->symbol,0),code[0]);
      break;
    case AST_ARRDECINIT:
      return tacJoin(tacJoin(tacCreate(TAC_VECINIT,ast->symbol,ast->son[1]->symbol,0,0),code[0]), code[2]);
      break;
    case AST_LISTINIT:
      return tacJoin(tacCreate(TAC_VECINITLIST, ast->son[0]->symbol,0,0,0),code[1]);
      break;
    case AST_FOR:
      return makeFor(ast->symbol,code[0],code[1],code[2],code[3],labelLoopEnd);
      break;
    case AST_BREAK:
      return makeBreak(labelLoopEnd);
      break;
    case AST_VARDECLST:
      return tacJoin(tacCreate(TAC_ARGDEC,ast->son[0]->symbol,0,0,0),code[1]);
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
	HASH_NODE* funcName = funcCall->symbol;
  AST* functionAst = funcName->funDeclNode;// Nodo da AST que possui a declaração da função sendo chamada
  AST* currentFuncArg = functionAst->son[1];// Nodo da lista de argumentos, será usado para colocar nome da var. da função no TAC_ARG
	for(buff = funcCall->son[0]; buff; buff = buff->son[1]){
    printf("Var de nome com funcall%s\n", currentFuncArg->son[0]->symbol->text);
		tacBuff = generateCode(buff->son[0],0); //expr or a symbol... tacGenerate can process
		tacArg = tacCreate(TAC_ARG, currentFuncArg->son[0]->symbol, tacBuff->res,funcName,i);
		params = tacJoin(tacJoin(params,tacBuff),tacArg);
		i++;
    currentFuncArg = currentFuncArg->son[1];// Pula para próximo elemento da lista de argumentos da definição da função
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

TAC* makeFunction(AST* funcAST,TAC* functionParams, TAC* functionCode){
  TAC* tacFuncStart = 0;
  TAC* tacFuncEnd = 0;
  tacFuncStart = tacCreate(TAC_BEGINFUN,funcAST->symbol,0,0,0);
  tacFuncEnd = tacCreate(TAC_ENDFUN,funcAST->symbol,0,0,0);
  return tacJoin(tacJoin(tacJoin(functionParams,tacFuncStart),functionCode),tacFuncEnd);
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
      if(buff->type == AST_PRINTSTR){
        tacPrint = tacCreate(TAC_PRINTSTR,tacBuff ? tacBuff->res: 0,0,0,0);
        result = tacJoin(tacJoin(result,tacBuff),tacPrint);
      }else if(buff->type == AST_PRINTEXP){
        tacPrint = tacCreate(TAC_PRINTEXP,tacBuff ? tacBuff->res: 0,0,0,0);
        result = tacJoin(tacJoin(result,tacBuff),tacPrint);
      }
			buff = buff->son[0];
		}else{
      printf("gerando cod printf\n");
			tacBuff = generateCode(buff->son[0],labelLoopEnd);
      if(buff->type == AST_PRINTSTR){
        tacPrint = tacCreate(TAC_PRINTSTR,tacBuff ? tacBuff->res: 0,0,0,0);
        result = tacJoin(tacJoin(result,tacBuff),tacPrint);
      }else if(buff->type == AST_PRINTEXP){
        tacPrint = tacCreate(TAC_PRINTEXP,tacBuff ? tacBuff->res: 0,0,0,0);
        result = tacJoin(tacJoin(result,tacBuff),tacPrint);
      }
			buff = buff->son[1];
		}

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

void generateASM(TAC* tac, FILE* fout){
  int stringPrintIndex;
  if(!tac){
    return;
  }
  if(tac->prev){
    generateASM(tac->prev,fout);
  }else{
    writeVars(fout);
    //fprintf(fout, "	.section\t__TEXT,__text,regular,pure_instructions\n\n");
  }

  switch(tac->type){
    case TAC_BEGINFUN:
      fprintf(fout,"## TAC_BEGINFUN\n"
        "\t.globl\t%s\n"
      "%s:\n",tac->res->text,tac->res->text);
      break;
    case TAC_ENDFUN:
      fprintf(fout,"## TAC_ENDFUN\n"
        "\tret\n");
      break;


    case TAC_SUB:
    //if(tac->op1->datatype==DATATYPE_INT && tac->op2->datatype==DATATYPE_INT){
        fprintf(fout, "## TAC_SUB\n"
          "\tmovl\t_%s(%%rip), %%eax\n"
          "\tsubl\t_%s(%%rip), %%eax\n"
          "\tmovl\t%%eax, _%s(%%rip)\n", tac->op1->text,tac->op2->text,tac->res->text);
      //}
        break;

    case TAC_VAR:
      writeVar(tac, fout);

      break;
    case TAC_VEC:

      fprintf(fout, "## TAC_VEC\n"
        "\t.comm _%s,%d\n",tac->res->text, (atoi(tac->op2->text) * 4));
      break;

    case TAC_VECINIT:
      fprintf(fout, "## TAC_ARRDECINIT\n"
        "\t.globl  _%s\n"
        "\t.data\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, %d\n"
      "_%s:\n",tac->res->text, tac->res->text, tac->res->text,(atoi(tac->op1->text) * 4),tac->res->text);
    break;

    case TAC_VECINITLIST:
      fprintf(fout,
        "\t.long %s\n",tac->res->text);
      // Essa tac precisa saber o tipo do vetor sendo declarado, para poder declarar o tipo certo
      break;

    case TAC_ARRWRITE:
      fprintf(fout, "## TAC_ARRWRITE tipos int\n"
        "\tmovl  _%s(%%rip), %%eax\n"// indice do vetor
        "\tmovl  _%s(%%rip), %%edx\n"//var da qual vem o dado 
        "\tcltq\n"
        "\tleaq  0(,%%rax,4), %%rcx\n"
        "\tleaq  _%s(%%rip), %%rax\n"//nome do vetor
        "\tmovl  %%edx, (%%rcx,%%rax)\n"
        "\tmovl  $0, %%eax\n",tac->op1->text, tac->op2->text, tac->res->text);
        break;

    case TAC_ARREF:
      fprintf(fout, "## TAC_ARREF tipos int\n"
        "\tmovl  _%s(%%rip), %%eax\n"
        "\tcltq\n"
        "\tleaq  0(,%%rax,4), %%rdx\n"
        "\tleaq  _%s(%%rip), %%rax\n"
        "\tmovl  (%%rdx,%%rax), %%eax\n"
        "\tmovl  %%eax, _%s(%%rip)\n"
        "\tmovl  $0, %%eax\n",tac->op2->text, tac->op1->text, tac->res->text);
      break;

    case TAC_PRINTSTR:
      printLabelCount--;
      fprintf(fout,"##TAC_PRINTSTR\n"
      "\tleaq  .LC%d(%%rip), %%rdi\n"
      "\tmovl  $0, %%eax\n"
      "\tcall  printf@PLT\n"
      "\tmovl  $0, %%eax\n",printLabelCount);

    break;

    case TAC_PRINTEXP:
      switch(tac->res->datatype){
        case DATATYPE_INT:
        fprintf(fout, "##TAC_PRINTEXP\n"
          "\tmovl  _%s(%%rip), %%eax\n"
          "\tmovl  %%eax, %%esi\n"
          "\tleaq  .LC0(%%rip), %%rdi\n"
          "\tmovl  $0, %%eax\n"
          "\tcall  printf@PLT\n"
          "\tmovl  $0, %%eax\n", tac->res->text);
        break;
        case DATATYPE_FLOAT:
        fprintf(fout, "##TAC_PRINTEXP\n"
          "\tsubq  $8, %%rsp\n"
          "\tmovss _%s(%%rip), %%xmm0\n"
          "\tcvtss2sd  %%xmm0, %%xmm0\n"
          "\tleaq  .LC1(%%rip), %%rdi\n"
          "\tmovl  $1, %%eax\n"
          "\tcall  printf@PLT\n"
          "\tmovl  $0, %%eax\n"
          "\taddq  $8, %%rsp\n", tac->res->text);
        break;
        case DATATYPE_BYTE:
        fprintf(fout, "##TAC_PRINTEXP\n"
          "\tsubq  $8, %%rsp\n"
          "\tmovzbl  _%s(%%rip), %%eax\n"
          "\tmovsbl  %%al, %%eax\n"
          "\tmovl  %%eax, %%esi\n"
          "\tleaq  .LC2(%%rip), %%rdi\n"
          "\tmovl  $0, %%eax\n"
          "\tcall  printf@PLT\n"
          "\tmovl  $0, %%eax\n"
          "\taddq  $8, %%rsp\n", tac->res->text);
        break;
        case DATATYPE_LONG:
        fprintf(fout, "##TAC_PRINTEXP\n"
          "\tsubq  $8, %%rsp\n"
          "\tmovq  _%s(%%rip), %%rax\n"
          "\tmovq  %%rax, %%rsi\n"
          "\tleaq  .LC3(%%rip), %%rdi\n"
          "\tmovl  $0, %%eax\n"
          "\tcall  printf@PLT\n"
          "\tmovl  $0, %%eax\n"
          "\taddq  $8, %%rsp\n", tac->res->text);
      }
      break;

    case TAC_MOVE:
      writeMove(tac->op1->text, tac->res->text, tac->op1->datatype, tac->res->datatype,fout);
      
      break;

    case TAC_ARG:
      fprintf(fout, "##TAC_ARG\n");
      writeMove(tac->op1->text, tac->res->text, tac->op1->datatype, tac->res->datatype,fout);
    break;

    case TAC_CALL:
      fprintf(fout, "##TAC_CALL\n"
        "\tmovl  $0, %%eax\n"
        "\tcall  %s\n"
        "\tmovl  %%eax, _%s(%%rip)\n"
        "\tmovl  $0, %%eax\n",tac->op1->text, tac->res->text);
      break;
    case TAC_RET:
      fprintf(fout, "##TAC_RET\n"
        "\tmovl  _%s(%%rip), %%eax\n"
        "\tret\n",tac->res->text);
      /*movss op1(%rip), %xmm0
  ret*/
      //TAC_RET precisa de uma maneira de saber o tipo da função para poder dar o return correto
      break;

    case TAC_LABEL:
      fprintf(fout, "##TAC_LABEL\n"
        ".%s:\n",tac->res->text);
    break;

    case TAC_JUMP:
      fprintf(fout, "##TAC_JUMP\n"
        "jmp .%s\n",tac->res->text);
    break;

    case TAC_IFZ:
      fprintf(fout, "##TAC_IFZ\n"
        "\tmovl  _%s(%%rip), %%eax\n"
        "\ttestl %%eax, %%eax\n"
        "\tje .%s\n",tac->op1->text,tac->res->text);
    break;

    case TAC_EQ:
      fprintf(fout, "##TAC_EQ\n"
        "\tmovl  _%s(%%rip), %%edx\n"
        "\tmovl  _%s(%%rip), %%eax\n"
        "\tcmpl  %%eax, %%edx\n"
        "\tsete  %%al\n"
        "\tmovzbl  %%al, %%eax\n"
        "\tmovl  %%eax, _%s(%%rip)\n"
        "\tmovl  $0, %%eax\n",tac->op1->text, tac->op2->text, tac->res->text);
    break;

    case TAC_ADD:
    case TAC_LESS:
    case TAC_GRE:
      writeBinOp(tac,fout);
      break;
    case TAC_READ:
      stringPrintIndex = 0;
      switch(tac->res->datatype){
        case DATATYPE_BOOL:
        case DATATYPE_INT:
          stringPrintIndex=0;
          break;
        case DATATYPE_FLOAT:
          stringPrintIndex = 1;
          break;
        case DATATYPE_BYTE:
          stringPrintIndex = 2;
          break;
        case DATATYPE_LONG:
          stringPrintIndex = 3;
          break;
      }
      fprintf(fout, 
        "\tsubq  $8, %%rsp\n"
        "\tleaq  _%s(%%rip), %%rsi\n"//var sendo escrita
        "\tleaq  .LC%d(%%rip), %%rdi\n"//ponteiro para string de leitura
        "\tmovl  $0, %%eax\n"
        "\tcall  __isoc99_scanf@PLT\n"
        "\tmovl  $0, %%eax\n"
        "\taddq  $8, %%rsp\n", tac->res->text, stringPrintIndex);
      break;
    default:
    break;
  }
}

void writeVar(TAC* tac, FILE* fout){

  char *varValue;
  char strZero[2] = "0";
  volatile float tempFloat;
  int tempInt;
  switch(tac->res->datatype){
    case DATATYPE_INT:
      if(tac->op1){
        varValue = tac->op1->text;
      }
      else varValue=strZero;
      fprintf(fout, "## TAC_VAR\n"
        "\t.globl  _%s\n"
        "\t.data\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long %s\n", tac->res->text, tac->res->text, tac->res->text, tac->res->text,varValue);  

    break;
    case DATATYPE_FLOAT:
      if(tac->op1){
        tempFloat = atof(tac->op1->text);
        tempInt = *(int*)&tempFloat;
      } else tempInt = 0;
      fprintf(fout, "## TAC_VAR float\n"
        ".globl\t_%s\n"
        "\t.data\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long %d\n"
        "##valor real:%f\n", tac->res->text, tac->res->text, tac->res->text,tac->res->text, tempInt,tempFloat);
      break;

    case DATATYPE_BOOL:
     if(tac->op1){//Se for uma variável delcarada como global
      if(tac->op1->text[0]=='T'){//Se o valor for true
        tempInt = 1;
      }else{// Se for false
        tempInt = 0;
      }

     }else tempInt = 0;
     fprintf(fout, "## TAC_VAR bool\n"
        "\t.globl  _%s\n"
        "\t.data\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 4\n"
      "_%s:\n"
        "\t.long %d\n", tac->res->text, tac->res->text, tac->res->text, tac->res->text,tempInt);
     break;

    case DATATYPE_BYTE:
      if(tac->op1){
        varValue = tac->op1->text;
      }
      else varValue=strZero;
      fprintf(fout, "## TAC_VAR\n"
        "\t.globl  _%s\n"
        "\t.data\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 1\n"
      "_%s:\n"
        "\t.byte %s\n", tac->res->text, tac->res->text, tac->res->text, tac->res->text,varValue);  
      break;

      case DATATYPE_LONG:
      if(tac->op1){
        varValue = tac->op1->text;
      }
      else varValue=strZero;
      fprintf(fout, "## TAC_VAR\n"
        "\t.globl  _%s\n"
        "\t.data\n"
        "\t.align 4\n"
        "\t.type _%s, @object\n"
        "\t.size _%s, 8\n"
      "_%s:\n"
        "\t.quad %s\n", tac->res->text, tac->res->text, tac->res->text, tac->res->text,varValue);  
      break;

  }

}

void writeFixed(TAC* first, FILE* output){
  TAC* tac;
  //int printLabelCount = 1;
  fprintf(output, ".LC0:\n"
          "\t.string \"%%d\"\n");
  fprintf(output, ".LC1:\n"
          "\t.string \"%%f\"\n");
  fprintf(output, ".LC2:\n"
          "\t.string \"%%c\"\n");
  fprintf(output, ".LC3:\n"
          "\t.string \"%%ld\"\n");   
  for(tac=first; tac; tac = tac->prev){
    if(tac->type == TAC_PRINTSTR){
      fprintf(output, ".LC%d:\n"
                  "\t.string %s\n",
              printLabelCount, tac->res->text); 
      printLabelCount++;
    }else if(tac->type ==TAC_ARGDEC){
      writeVar(tac, output);
    }

  }
}

void writeMove(char* source, char* target, int sourceDatatype, int targetDatatype, FILE* fout){
  fprintf(fout, "##TAC_MOVE INTs\n"
        "\tmovl  _%s(%%rip), %%eax\n"
        "\tmovl  %%eax, _%s(%%rip)\n"
        "\tmovl  $0, %%eax\n",source, target);
}


void writeBinOp(TAC* operation, FILE* fout){
  fprintf(fout, "##Binop\n");
  /*Switch para escrever o mov do espaço de memória do dado para
    O registrador*/
  switch(operation->op1->datatype){
    case DATATYPE_INT:
    case DATATYPE_BOOL:
      fprintf(fout,"\tmovl  _%s(%%rip), %%edx\n",operation->op1->text);
      if(operation->op2->datatype == DATATYPE_FLOAT){// Se a outra variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ss  %%edx, %%xmm1\n");
        // No compilador, se a operação for int x float, ele muda o
        //registrador para eax. O reg é edx aqui por consistência.
      }
      break;

    case DATATYPE_BYTE:
      fprintf(fout,"\tmovzbl  _%s(%%rip), %%eax\n"
        "\tmovsbl  %%al, %%edx\n",operation->op1->text);

      if(operation->op2->datatype == DATATYPE_FLOAT){// Se a outra variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ss  %%edx, %%xmm1\n");
        // No compilador, se a operação for char x float, ele muda o
        //registrador para eax. O reg é edx aqui por consistência.
      }
      break;
    case DATATYPE_LONG:
      fprintf(fout,"\tmovq  _%s(%%rip), %%rax\n"
        ,operation->op1->text);
      if(operation->op2->datatype == DATATYPE_FLOAT){// Se a outra variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ssq %%rax, %%xmm1\n");
      }else{
        fprintf(fout,"\tmovl  %%eax, %%edx\n");// Se não for, fazer esse mov
      }
      break;
    case DATATYPE_FLOAT:
      fprintf(fout,"\tmovss _%s(%%rip), %%xmm1\n",operation->op1->text);
      break;
  }

  switch(operation->op2->datatype){
    case DATATYPE_INT:
    case DATATYPE_BOOL:
      fprintf(fout,"\tmovl  _%s(%%rip), %%eax\n",operation->op2->text);
      if(operation->op1->datatype == DATATYPE_FLOAT){// Se a primeira variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ss %%eax, %%xmm0\n");
      }
      break;
    case DATATYPE_BYTE:
      fprintf(fout,"\tmovzbl  _%s(%%rip), %%eax\n"
        "\tmovsbl  %%al, %%eax\n",operation->op2->text);
      if(operation->op1->datatype == DATATYPE_FLOAT){// Se a primeira variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ss  %%eax, %%xmm0\n");
      }
      break;
    case DATATYPE_LONG:
      fprintf(fout,"\tmovq  _%s(%%rip), %%rax\n",operation->op2->text);
      if(operation->op1->datatype == DATATYPE_FLOAT){// Se a primeira variável for float, fazer cast para float
        fprintf(fout,"\tcvtsi2ssq %%rax, %%xmm0\n");
      }
      break;
    case DATATYPE_FLOAT:
      fprintf(fout,"\tmovss _%s(%%rip), %%xmm0\n",operation->op2->text);
      break;
  }


  switch(operation->type){
    case TAC_ADD:
      if((operation->op1->datatype != DATATYPE_FLOAT) && (operation->op2->datatype != DATATYPE_FLOAT)){
        fprintf(fout,
          "\taddl  %%edx, %%eax\n"
          "\tmovl  %%eax, _%s(%%rip)\n",operation->res->text);
      }else{
        fprintf(fout,
          "\taddss %%xmm1, %%xmm0\n"
          "\tcvttss2si %%xmm0, %%eax\n"
          "\tmovl %%eax, _%s(%%rip)\n",operation->res->text);
      }
      break;

    case TAC_GRE:
      if((operation->op1->datatype != DATATYPE_FLOAT) && (operation->op2->datatype != DATATYPE_FLOAT)){
        fprintf(fout, 
          "\tcmpl  %%eax, %%edx\n"
          "\tsetg  %%al\n"
          "\tmovzbl  %%al, %%eax\n"
          "\tmovl  %%eax, _%s(%%rip)\n"
          "\tmovl  $0, %%eax\n", operation->res->text);
      }else{
        fprintf(fout,
          "\tucomiss %%xmm0, %%xmm1\n"
          "\tseta  %%al\n"
          "\tmovzbl  %%al, %%eax\n"
          "\tmovl  %%eax, _%s(%%rip)\n", operation->res->text);
      }
    case TAC_LESS:
      if((operation->op1->datatype != DATATYPE_FLOAT) && (operation->op2->datatype != DATATYPE_FLOAT)){
        fprintf(fout, 
          "\tcmpl  %%eax, %%edx\n"
          "\tsetl  %%al\n"
          "\tmovzbl  %%al, %%eax\n"
          "\tmovl  %%eax, _%s(%%rip)\n"
          "\tmovl  $0, %%eax\n", operation->res->text);
      }else{
        fprintf(fout,
          "\tucomiss %%xmm1, %%xmm0\n"
          "\tseta  %%al\n"
          "\tmovzbl  %%al, %%eax\n"
          "\tmovl  %%eax, _%s(%%rip)\n", operation->res->text);
      }
    break;
    default:
      fprintf(fout,"##Operação não implementada\n");
      break;
  }
  

}
