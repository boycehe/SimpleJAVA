#include <stdio.h>
#include "simple_java_foundation.h"

void createClass(JavaParser *parser,Token *clsToken,Token *parentClasToken,JavaClassBody *clsBody);
JavaClassBody *dressedClassBodyWithVars(JavaClassBody *classBody,ExprList*vars);
JavaClassBody *dressedClassBodyWithFunc(JavaClassBody *classBody,JavaFunction*func);
JavaFunction *addFunctionToClass(JavaParser *parser,JavaAccessType accessType,Token *returnType,Token *name,ExprList *parametreList,JavaSentenceList *sentenceList);
void addExprToFunc(JavaFunction *func,JavaSentence *senctence);
