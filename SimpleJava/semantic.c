#include <stdio.h>
#include "semantic.h"
#include "tokenize.h"
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
	expr->tokenType = op;
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

JavaFunction *createFunction(JavaParser *parser,JavaAccessType accessType,int returnType,Token token,JavaParameterlist *parameters,JavaExprList *functionBody){
    
    JavaFunction *func = (JavaFunction *)malloc(sizeof(JavaFunction));
    func->accessType = accessType;
    func->returnType = returnType;
    func->name = (char *)malloc(sizeof(char)*token.n);
    memcpy(func->name, token.z, token.n);
    func->parameters = parameters;
    func->sentenceList = functionBody;
    
    if (strncmp(token.z, "main",token.n) == 0) {
        parser->entryFunc = func;
        return 0;
    }
    
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
    expr->type = ExprType_VAR;
    expr->declareType = HC_ID;
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
        paramList->parmeters =(JavaParameter **)malloc(sizeof(JavaParameter *)*10);
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

JavaClassItems *dressedClassBodyWithFunc(JavaClassItems *clsItem,JavaFunction*javaFunc){
    
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
    
    if (clsItem->functions->capity <= clsItem->functions->len) {
        clsItem->functions->funcs = (JavaFunction **)realloc(clsItem->functions->funcs, clsItem->functions->capity + 10);
        clsItem->functions->capity += 10;
    }
    
    memcpy((clsItem->functions->funcs + clsItem->functions->len), &javaFunc, sizeof(JavaFunction *));
    clsItem->functions->len++;
    
    return clsItem;
    
}

JavaExpr *parameterToExpr(JavaParameter *parmaeter){
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->type = ExprType_VAR;
    expr->token = parmaeter->token;
    expr->declareType = parmaeter->type;
    return expr;
}

int equalToken(Token *leftToken,Token *rightToken){
    
    if (leftToken->n != rightToken->n) {
        return 0;
    }
    
    for (int i = 0; i < leftToken->n; i++) {
        
        if (leftToken->z[i] != rightToken->z[i]) {
            return 0;
        }
        
    }
    
    return 1;
    
}

JavaExpr *findClassInstanceVarExpr(MetaJavaClass *clsInstance ,Token *varToken){
    
    if (clsInstance->clsItems == NULL || clsInstance->clsItems->declareVarList == NULL) {
        return 0;
    }
    JavaExpr *findVarExpr = 0;
               
    for (int i = 0; i < clsInstance->clsItems->declareVarList->len; i++) {
                   
         JavaExpr *clsInstanceVar = *(clsInstance->clsItems->declareVarList->exprs + i);
                   
         if (equalToken(clsInstanceVar->token, varToken) == 1) {
            findVarExpr = clsInstanceVar;
         }
                   
    }
               
    if (findVarExpr != 0 ) {
        return findVarExpr;
    }
    
    if (clsInstance->superClass != 0) {
        return findClassInstanceVarExpr(clsInstance->superClass, varToken);
    }
    
    return 0;
}

JavaExpr *findVarExpr(JavaExprList *exprlist,Token *clsToken,Token *varToken){
    
    if (exprlist == 0 || varToken == 0) {
        return 0;
    }
    
    JavaExpr *findVarExpr = 0;
    
    if (clsToken  == 0) {
        
        for (int i = 0; i < exprlist->len; i++) {
    
            JavaExpr *expr = *(exprlist->exprs + i);
            
            if (equalToken(expr->token, varToken) == 1) {
                findVarExpr = expr;
                break;
            }
        
        }
        
    }else{
        
        for (int i = 0; i < exprlist->len; i++) {
        
            JavaExpr *expr = *(exprlist->exprs + i);
            if (expr->type != ExprType_CLSINSTANCE) {
                continue;
            }
            
            if (strncmp(expr->clsInstance->clsName, clsToken->z, clsToken->n) != 0) {
                continue;
            }
            
            if (expr->clsInstance->clsItems == NULL || expr->clsInstance->clsItems->declareVarList == NULL) {
                return 0;
            }
            
            findVarExpr = findClassInstanceVarExpr(expr->clsInstance,varToken);
            

        }
        
    }
    
    return findVarExpr;
}

JavaClassItems *copyClassItems(JavaClassItems *clsItems){
    
    return 0;
}

JavaFunctionList *copyFunctionList(JavaFunctionList *functionList){
    return 0;
}

JavaExprList *copyExprList(JavaExprList *exprList){
    return 0;
}



MetaJavaClass *newInstanceClass(JavaParser *parser, Token *clsToken){
    
    MetaJavaClass *metaCls = findClssWithToken(parser, *clsToken);
    
    if (metaCls == 0) {
        return 0;
    }
    
    MetaJavaClass *cls = (MetaJavaClass *)malloc(sizeof(MetaJavaClass));
    cls->clsName = (char *)malloc(sizeof(clsToken->n+1));
    memset(cls->clsName, '\0', sizeof(clsToken->n+1));
    memcpy(cls->clsName, clsToken->z, clsToken->n);
    
    /*
     struct JavaClassItems {
         
         JavaFunctionList *functions;
         JavaExprList *declareVarList;
         
     };
     */
    
    cls->clsItems = (JavaClassItems *)malloc(sizeof(JavaClassItems));
    
    
    
    return cls;
    
    
}

JavaExpr *runFunc(JavaParser *parser,MetaJavaClass *cls,CallTokensList *tokenList,JavaFunction *func){
    
    unsigned short capity = 10;
    
    JavaExprList *tmpVarList = (JavaExprList *)malloc(sizeof(JavaExprList));
    
    //检查方法调用是否正确
    //无参数

    if (tokenList == 0) {
       
       if ((func->parameters != 0 && func->parameters->len >= 0)) {
            return 0;
       }
    
    }else if(func->parameters == 0 || func->parameters->len != tokenList->len) {
        return 0;
    }
    
    
    
    if (func->parameters != NULL && func->parameters->parmeters != 0) {
    
        if (capity < func->parameters->len) {
            capity = func->parameters->len;
        }
        tmpVarList->len = 0;
        tmpVarList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr *)*capity);
        tmpVarList->capity = capity;
    
        for (int i = 0; i < func->parameters->len; i++) {
            
            JavaParameter *par = *(func->parameters->parmeters+i);
            JavaExpr *expr = parameterToExpr(par);
            expr->valueToken = (*(tokenList->exprs+i))->token;
            expr->valueTokenType= (*(tokenList->exprs+i))->tokenType;
            memcpy((tmpVarList->exprs+i), &expr, sizeof(JavaExpr*));
            tmpVarList->len++;
        }
        
    }else{
        tmpVarList->len = 0;
        tmpVarList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr *)*capity);
        tmpVarList->capity = capity;
    }
    
    for (int i = 0; i < func->sentenceList->len; i++) {
        
        JavaExpr *expr = *(func->sentenceList->exprs + i);
        
        switch (expr->type) {
            case ExprType_ASSGIN:
            {
                JavaExpr *leftExpr = expr->assginLeftExpr;
                JavaExpr *rightExpr = expr->assginRightExpr;
                
                if (leftExpr->tokenType == HC_ID) {
                    JavaExpr *clsInstaceVar = (JavaExpr *)malloc(sizeof(JavaExpr));
                    clsInstaceVar->type = ExprType_CLSINSTANCE;
                    clsInstaceVar->clsInstance = (MetaJavaClass *)malloc(sizeof(MetaJavaClass));
                    MetaJavaClass *cls = findClssWithToken(parser, *leftExpr->token);
                    if (cls == NULL) {
                        char *errormsg = (char *)malloc(sizeof(char)*(leftExpr->token->n+1));
                        memset(errormsg, '\0', (leftExpr->token->n+1));
                        memcpy(errormsg, leftExpr->token->z, leftExpr->token->n);
                        printf("Undefined clss %s",errormsg);
                        free(errormsg);
                        return NULL;
                    }
                    clsInstaceVar->clsInstance->superClass = cls->superClass;
                    clsInstaceVar->clsInstance->clsName = cls->clsName;
                    memcpy((tmpVarList->exprs+tmpVarList->len), &clsInstaceVar, sizeof(JavaExpr*));
                    tmpVarList->len++;
                }else{
                    
                    
                    
                    
                }
                
                
                
            }
                continue;
            case ExprType_VAR:
            {
                
            }
                continue;
            case ExprType_THISTOKEN:
            {
                
            }
                continue;
            case ExprType_NEW:
            {
                
            }
                continue;
            case ExprType_RETURN:
            {
                
            }
                break;
            default:
            {
                printf("invaild secentence!\n");
            }
                break;
        }
        
        
    }
    
    
     return NULL;
}

void javaRun(JavaParser *parser){
    
    if (parser->entryFunc ==  0) {
        printf("can't find main function\n");
        return;
    }
    
    runFunc(parser, 0, 0,parser->entryFunc);
}


