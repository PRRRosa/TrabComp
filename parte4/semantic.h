#include "astree.h"
void checkAndSetTypes(AST*node);
void checkUndeclared();
int getSemanticError();
void checkOperands(AST*node);
int checkFuncParameters(AST* funcCallNode);
