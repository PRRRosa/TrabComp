#include "astree.h"
void checkAndSetTypes(AST*node);
void checkUndeclared();
int getSemanticError();
void checkOperands(AST*node, char*currentFunction);
int checkFuncParameters(AST* funcCallNode);
int isNodeTypeNumber(AST* node);
int isNodeTypeBool(AST* node);
int checkFuncParameter(AST* funDefParameter, AST* funCallParameter);
void checkArrayElements(AST* n, int tipo);
