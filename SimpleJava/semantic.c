#include <stdio.h>
#include "semantic.h"
#include <stdlib.h>
#include <String.h>

Token *pointToken(Token token){
    
	Token *ntoken = (Token*)malloc(sizeof(Token));
	ntoken->z = token.z;
	ntoken->n = token.n;
	return ntoken;
}

JavaExpr *tokenToExpr(Token token ,int op){
	JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
	expr->token = pointToken(token);
	expr->type = op;
	return  expr;
}

MetaJavaClass *findClssWithToken(JavaParser *parser,Token token){
    
    if (parser == NULL || parser->classes == NULL || parser->clsLen <= 0) {
        return NULL;
    }
    
    for (int i = 0; i < parser->clsLen; i++) {
        
        MetaJavaClass *cls = *(parser->classes + i);
        if (strncmp(cls->clsName, token.z, token.n)==0) {
            return  cls;
        }
    
    }
    
    return NULL;

}

void createClass(JavaParser *parser,Token clsToken,Token parentClasToken,JavaClassItems *clsItems){
    
    if (parser == NULL) {
        parser = (JavaParser *)malloc(sizeof(JavaParser));
    }
    
    MetaJavaClass *cls = (MetaJavaClass *)malloc(sizeof(MetaJavaClass));
    if (parentClasToken.n != 0 && parentClasToken.z != NULL) {
       cls->superClass = findClssWithToken(parser, parentClasToken);
    }
    
    cls->clsItems = clsItems;
    
}

JavaExpr *instanceGetProperty(Token token,Token propertyToken){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->token = pointToken(token);
    expr->propertyToken = pointToken(propertyToken);

    return expr;
    
}

JavaExpr *dressedCallExpr(Token clsInstanceToken,Token funcNameToken,CallTokensList *parameterList){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    
    if (clsInstanceToken.n > 0 && clsInstanceToken.z != NULL) {
        expr->token = pointToken(clsInstanceToken);
        expr->propertyToken = pointToken(funcNameToken);
    }else{
        expr->token = pointToken(funcNameToken);
    }
    
    expr->callParList = parameterList;
        
    return expr;
    
}

CallTokensList *addTokenToCallParameterList(CallTokensList *parameterList,JavaExpr *expr){
    
    if (parameterList == NULL) {
        parameterList = (CallTokensList *)malloc(sizeof(CallTokensList));
        parameterList->capity = 10;
        parameterList->len = 0;
        parameterList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
    }
    
    if (parameterList->capity <= parameterList->len) {
        parameterList->exprs = (JavaExpr **)realloc(parameterList->exprs, parameterList->capity + 10);
        parameterList->capity += 10;
    }
    memcpy((parameterList->exprs + parameterList->len), &expr, sizeof(JavaExpr *));
    parameterList->len++;
    return parameterList;
    
}

JavaExpr *assginmentExpr(JavaExpr *leftExpr,JavaExpr *rightExpr){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->type = ExprType_ASSGIN;
    expr->assginLeftExpr = leftExpr;
    expr->assginRightExpr = rightExpr;
    return expr;
    
}

JavaExpr *addDeclarevarItem(JavaExpr *declareExpr,JavaExpr *itemExpr){
    
    if (declareExpr == NULL) {
        declareExpr = (JavaExpr *)malloc(sizeof(JavaExpr));
        declareExpr->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        declareExpr->exprLen = 0;
        declareExpr->exprCapity = 10;
    }
    
    if (declareExpr->exprCapity <= declareExpr->exprLen) {
        declareExpr->exprs = (JavaExpr **)realloc(declareExpr->exprs, declareExpr->exprCapity + 10);
        declareExpr->exprCapity += 10;
    }
    
    memcpy((declareExpr->exprs + declareExpr->exprLen), &itemExpr, sizeof(JavaExpr *));
    declareExpr->exprLen++;
    
    return declareExpr;
}

JavaExpr *finishDeclareVars(int declareType,JavaExpr *declareExpr,Token token,int tokenType){
    
    if (declareExpr == NULL) {
        declareExpr = (JavaExpr *)malloc(sizeof(JavaExpr));
        declareExpr->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        declareExpr->exprLen = 0;
        declareExpr->exprCapity = 10;
    }
    
    if (declareExpr->exprCapity <= declareExpr->exprLen) {
        declareExpr->exprs = (JavaExpr **)realloc(declareExpr->exprs, declareExpr->exprCapity + 10);
        declareExpr->exprCapity += 10;
    }
    
    declareExpr->type = ExprType_DECLARE;
    declareExpr->declareType = tokenType;
    JavaExpr *itemExpr = tokenToExpr(token, tokenType);
    memcpy((declareExpr->exprs + declareExpr->exprLen), &itemExpr, sizeof(JavaExpr *));
    declareExpr->exprLen++;
    
    return declareExpr;
    
    
    
}

JavaFunction *createFunction(JavaAccessType accessType,int returnType,Token token,JavaParameterlist *parameters,JavaExprList *functionBody){
    
    JavaFunction *func = (JavaFunction *)malloc(sizeof(JavaFunction));
    func->accessType = accessType;
    func->returnType = returnType;
    func->name = (char *)malloc(sizeof(char)*token.n);
    memcpy(func->name, token.z, token.n);
    func->parameters = parameters;
    func->sentenceList = functionBody;
    
    return func;
    
}

JavaExprList *addExprToList(JavaExprList *exprList,JavaExpr *expr){
    
   if (exprList == NULL) {
        exprList = (JavaExprList *)malloc(sizeof(JavaExprList));
        exprList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        exprList->len = 0;
        exprList->capity = 10;
    }
    
    if (exprList->capity <= exprList->len) {
        exprList->exprs = (JavaExpr **)realloc(exprList->exprs, exprList->capity + 10);
        exprList->capity += 10;
    }
    
    memcpy((exprList->exprs + exprList->len), &expr, sizeof(JavaExpr *));
    exprList->len++;
    
    return exprList;
    
}

JavaExpr *declareClassVar(Token clsToken,Token clsVar){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->type = ExprType_CLSVAR;
    expr->token = pointToken(clsToken);
    expr->varToken = pointToken(clsVar);
    return expr;
    
}

JavaExpr *getClassProperty(Token token){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->token = pointToken(token);
    expr->type = ExprType_THISTOKEN;
    return expr;
    
}

JavaExpr *newClsInstance(Token clsToken){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->type = ExprType_NEW;
    expr->token = pointToken(clsToken);
    return expr;
    
}

JavaExpr *returnExpr(JavaExpr *expr){
    JavaExpr *returnExpr = (JavaExpr *)malloc(sizeof(JavaExpr));
    returnExpr->type = ExprType_RETURN;
    returnExpr->returnExpr = expr;
    return expr;
}

JavaParameterlist *addParameter(JavaParameterlist *paramList,int tokenType,Token token){
    if (paramList == NULL) {
        paramList = (JavaParameterlist *)malloc(sizeof(JavaParameterlist));
        paramList->capity = 10;
        paramList->len = 0;
    }
    
    if (paramList->capity <= paramList->len) {
        paramList->parmeters = (JavaParameter **)realloc(paramList->parmeters, paramList->capity + 10);
        paramList->capity += 10;
    }
    JavaParameter *parameter = (JavaParameter *)malloc(sizeof(JavaParameter));
    parameter->type = tokenType;
    parameter->token = pointToken(token);
    memcpy((paramList->parmeters + paramList->len), &parameter, sizeof(JavaParameter *));
    paramList->len++;
    
    return paramList;
}

JavaClassItems *dressedClassBodyWithVars(JavaClassItems *clsItem,JavaExpr*varExpr){
    
    if (clsItem == NULL) {
        clsItem = (JavaClassItems *)malloc(sizeof(JavaClassItems));
        clsItem->functions = (JavaFunctionList *)malloc(sizeof(JavaFunctionList));
        clsItem->functions->funcs = (JavaFunction **)malloc(sizeof(JavaFunction*)*10);
        clsItem->functions->capity = 10;
        clsItem->functions->len = 0;
        
        clsItem->declareVarList = (JavaExprList *)malloc(sizeof(JavaExprList));
        clsItem->declareVarList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        clsItem->declareVarList->capity = 10;
        clsItem->declareVarList->len = 0;
    }
    
    if (clsItem->declareVarList->capity <= clsItem->declareVarList->len) {
        clsItem->declareVarList->exprs = (JavaExpr **)realloc(clsItem->declareVarList->exprs, clsItem->declareVarList->capity + 10);
        clsItem->declareVarList->capity += 10;
    }
    
    memcpy((clsItem->declareVarList->exprs + clsItem->declareVarList->len), &varExpr, sizeof(JavaExpr *));
    clsItem->declareVarList->len++;
    
    return clsItem;
    
}

JavaClassItems *dressedClassBodyWithFunc(JavaClassItems *clsItem,JavaExpr*varExpr){
    
    if (clsItem == NULL) {
        clsItem = (JavaClassItems *)malloc(sizeof(JavaClassItems));
        clsItem->functions = (JavaFunctionList *)malloc(sizeof(JavaFunctionList));
        clsItem->functions->funcs = (JavaFunction **)malloc(sizeof(JavaFunction*)*10);
        clsItem->functions->capity = 10;
        clsItem->functions->len = 0;
        
        clsItem->declareVarList = (JavaExprList *)malloc(sizeof(JavaExprList));
        clsItem->declareVarList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        clsItem->declareVarList->capity = 10;
        clsItem->declareVarList->len = 0;
    }
    
    if (clsItem->functions->capity <= clsItem->declareVarList->len) {
        clsItem->functions->funcs = (JavaFunction **)realloc(clsItem->functions->funcs, clsItem->functions->capity + 10);
        clsItem->functions->capity += 10;
    }
    
    memcpy((clsItem->functions->funcs + clsItem->functions->len), &varExpr, sizeof(JavaFunction *));
    clsItem->functions->len++;
    
    return clsItem;
    
}


