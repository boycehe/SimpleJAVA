#include <stdio.h>
#include "simple_java_foundation.h"

void createClass(JavaParser *parser,Token clsToken,Token parentClasToken,JavaClassItems *clsBody);
JavaExpr *tokenToExpr(Token token ,int op);
JavaExpr *instanceGetProperty(Token token,Token propertyToken);
JavaExpr *dressedCallExpr(Token clsInstanceToken,Token funcNameToken,CallTokensList *parameterList);
CallTokensList *addTokenToCallParameterList(CallTokensList *parameterList,JavaExpr *expr);
JavaExpr *assginmentExpr(JavaExpr *leftExpr,JavaExpr *rightExpr);
JavaExpr *addDeclarevarItem(JavaExpr *declareExpr,JavaExpr *itemExpr);
JavaFunction *createFunction(JavaParser *parser,JavaAccessType accessType,int returnType,Token token,JavaParameterlist *parameters,JavaExprList *functionBody);
JavaExprList *addExprToList(JavaExprList *exprList,JavaExpr *expr);
JavaExpr *declareClassVar(Token clsToken,Token clsVar);
JavaExpr *getClassProperty(Token token);
JavaExpr *newClsInstance(Token clsToken);
JavaExpr *returnExpr(JavaExpr *expr);
JavaParameterlist *addParameter(JavaParameterlist *paramList,int tokenType,Token token);
JavaClassItems *dressedClassBodyWithVars(JavaClassItems *clsItem,JavaExpr*varExpr);
JavaClassItems *dressedClassBodyWithFunc(JavaClassItems *clsItem,JavaFunction*javaFunc);
JavaExpr *finishDeclareVars(int declareType,JavaExpr *declareExpr,Token token,int tokenType);

void javaRun(JavaParser *parser);
