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

JavaExpr *initExpr(){
    
    JavaExpr *expr = (JavaExpr *)malloc(sizeof(JavaExpr));
    expr->type = 0;
    expr->declareType = HC_ILLEGAL;
    expr->token = 0;
    expr->tokenType = HC_ILLEGAL;
    expr->propertyToken = 0;
    expr->varToken = 0;
    expr->valueToken = 0;
    expr->valueTokenType = HC_ILLEGAL;
    expr->callParList = 0;
    expr->assginLeftExpr = 0;
    expr->assginRightExpr = 0;
    expr->exprs = 0;
    expr->returnExpr = 0;
    expr->clsInstance = 0;
    expr->exprLen = 0;
    expr->exprCapity =0;
    
    return expr;
    
}

JavaExpr *varTokenToExpr(Token token ,int type){
    
    JavaExpr *expr = initExpr();
   
    if (type == HC_INTEGER || type == HC_TEXT) {
        expr->type = ExprType_CONSTANT;
        expr->token = pointToken(token);
        expr->tokenType = type;
    }else{
        expr->type = ExprType_VAR;
        expr->varToken = pointToken(token);
    }
    
    return expr;
    
}

JavaExpr *tokenToExpr(Token token ,int op){
    JavaExpr *expr = initExpr();
	expr->token = pointToken(token);
	expr->tokenType = op;
    expr->type = ExprType_TOKEN;
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
    
    if (clsToken.n == 0) {
        return;
    }
    
    MetaJavaClass *cls = (MetaJavaClass *)malloc(sizeof(MetaJavaClass));
    if (parentClasToken.n != 0 && parentClasToken.z != NULL) {
       cls->superClass = findClssWithToken(parser, parentClasToken);
    }else{
       cls->superClass = 0;
    }
    cls->clsName = (char *)malloc(sizeof(char)*(clsToken.n+1));
    memcpy(cls->clsName, clsToken.z, clsToken.n);
    cls->clsItems = clsItems;
    if (parser->clsLen + 1 >= parser->clsCapity) {
        parser->classes = (MetaJavaClass **)realloc(parser->classes, sizeof(MetaJavaClass*)*(parser->clsCapity+10));
        parser->clsCapity += 10;
    }
    
    memcpy((parser->classes + parser->clsLen), &cls, sizeof(MetaJavaClass*));
    parser->clsLen++;
    
}

JavaExpr *instanceGetProperty(int tokenType,Token token,Token propertyToken){
    
    JavaExpr *expr = initExpr();
    if (tokenType == HC_THIS) {
        expr->type = ExprType_THISTOKEN;
    }else{
        expr->type = ExprType_VAR;
    }
    expr->token = pointToken(token);
    expr->varToken = pointToken(propertyToken);
    return expr;
    
}

JavaExpr *dressedCallExpr(Token clsInstanceToken,Token funcNameToken,CallTokensList *parameterList){
    
    
    JavaExpr *expr = initExpr();
    expr->type = ExprType_CALL;
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
    
    JavaExpr *expr = initExpr();
    expr->type = ExprType_ASSGIN;
    expr->assginLeftExpr = leftExpr;
    expr->assginRightExpr = rightExpr;
    return expr;
    
}

JavaExpr *addDeclarevarItem(JavaExpr *declareExpr,JavaExpr *itemExpr){
    
    if (declareExpr == NULL) {
        declareExpr = initExpr();
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

JavaExpr *finishDeclareVars(int declareType,JavaExpr *declareExpr,JavaExpr *expr){
    
    if (declareExpr == NULL) {
        declareExpr = initExpr();
        declareExpr->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*10);
        declareExpr->exprLen = 0;
        declareExpr->exprCapity = 10;
    }
    
    if (declareExpr->exprCapity <= declareExpr->exprLen) {
        declareExpr->exprs = (JavaExpr **)realloc(declareExpr->exprs, declareExpr->exprCapity + 10);
        declareExpr->exprCapity += 10;
    }
    
    declareExpr->type = ExprType_DECLARE;
    declareExpr->declareType = declareType;

    memcpy((declareExpr->exprs + declareExpr->exprLen), &expr, sizeof(JavaExpr *));
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
    
    JavaExpr *expr = initExpr();
    expr->type = ExprType_VAR;
    expr->declareType = HC_ID;
    expr->token = pointToken(clsToken);
    expr->varToken = pointToken(clsVar);
    return expr;
    
}

JavaExpr *getClassProperty(Token token){
    
    JavaExpr *expr = initExpr();
    expr->varToken = pointToken(token);
    expr->type = ExprType_THISTOKEN;
    return expr;
    
}

JavaExpr *newClsInstance(Token clsToken){
    
    JavaExpr *expr = initExpr();
    expr->type = ExprType_NEW;
    expr->token = pointToken(clsToken);
    return expr;
    
}

JavaExpr *returnExpr(JavaExpr *expr){
    JavaExpr *returnExpr = initExpr();
    returnExpr->type = ExprType_RETURN;
    returnExpr->returnExpr = expr;
    return returnExpr;
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
    JavaExpr *expr = initExpr();
    expr->type = ExprType_VAR;
    expr->varToken = parmaeter->token;
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
        
        for (int j = 0; j < clsInstanceVar->exprLen; j++) {
            JavaExpr *varExpr = *(clsInstanceVar->exprs + j);
            if (equalToken(varExpr->varToken, varToken) == 1) {
                findVarExpr = varExpr;
                break;
            }
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

JavaFunction *findInstanceFunc(MetaJavaClass *classInstance,Token *funcToken){
    
    if (classInstance->clsItems == 0 || classInstance->clsItems->functions == 0) {
        return 0;
    }
    
    JavaFunction *func = 0;
    
    for (int i = 0; i < classInstance->clsItems->functions->len; i++) {
        
        JavaFunction *localFunc = *(classInstance->clsItems->functions->funcs + i);
        
        if (strncmp(funcToken->z, localFunc->name, funcToken->n) != 0) {
            continue;
        }
        
        func = localFunc;
    
    }
    
    if (func == 0 && classInstance->superClass != 0) {
        return findInstanceFunc(classInstance->superClass, funcToken);
    }

    return func;
    
};

JavaFunction *findClsFunc(JavaExprList *exprlist,Token *clsToken,Token *funcToken){
                          
    JavaFunction *func = 0;
                          
    for (int i = 0; i < exprlist->len; i++) {
                                 
        JavaExpr *expr = *(exprlist->exprs + i);
        if (expr->type != ExprType_CLSINSTANCE) {
            continue;
        }
                                     
        if (equalToken(clsToken, expr->token) != 1) {
            continue;
        }
                                     
        if (expr->clsInstance->clsItems == NULL || expr->clsInstance->clsItems->functions == NULL) {
            return 0;
        }
                                     
        func = findInstanceFunc(expr->clsInstance,funcToken);
                                     
    }
                          
     return func;
                          
}

JavaExpr *findVarExpr(JavaExprList *exprlist,Token *clsToken,Token *varToken){
    
    if (exprlist == 0 || (clsToken == 0 && varToken == 0)) {
        return 0;
    }
    
    JavaExpr *findVarExpr = 0;
    
    if (clsToken  == 0) {
        
        for (int i = 0; i < exprlist->len; i++) {
    
            JavaExpr *expr = *(exprlist->exprs + i);
            
            if (equalToken(expr->varToken, varToken) == 1) {
                findVarExpr = expr;
                break;
            }
        
        }
        
    }else if(varToken == 0){
        
        for (int i = 0; i < exprlist->len; i++) {
            
            JavaExpr *expr = *(exprlist->exprs + i);
            if (expr->type != ExprType_CLSINSTANCE) {
                continue;
            }
                
            if (equalToken(clsToken, expr->varToken) != 1) {
                continue;
            }
        
            findVarExpr = expr;
            break;
                
        }
        
        
    }else{
    
        for (int i = 0; i < exprlist->len; i++) {
            
            JavaExpr *expr = *(exprlist->exprs + i);
            if (expr->type != ExprType_CLSINSTANCE) {
                continue;
            }
                
            if (equalToken(clsToken, expr->varToken) != 1) {
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


Token *copyToken(Token *oldToken){
    
    if (oldToken == 0) {
        return 0;
    }
    
    Token *newToken = (Token *)malloc(sizeof(Token));
    newToken->z = (const unsigned char *)malloc(oldToken->n+1);
    memset(newToken->z, '\0', oldToken->n);
    memcpy(newToken->z, oldToken->z, oldToken->n);
    return newToken;
}


JavaExpr *copyExpr(JavaExpr *oldExpr){
    
    JavaExpr *newExpr = initExpr();
    newExpr->type = oldExpr->type;
    newExpr->declareType = oldExpr->declareType;
    newExpr->token = copyToken(oldExpr->token);
    newExpr->tokenType = oldExpr->tokenType;
    newExpr->varToken = copyToken(oldExpr->varToken);
    newExpr->valueToken = copyToken(oldExpr->valueToken);
    newExpr->valueTokenType = oldExpr->valueTokenType;
    
    return newExpr;
    
}

JavaExprList *copyExprList(JavaExprList *exprList){
    
    JavaExprList *newExprList = (JavaExprList *)malloc(sizeof(JavaExprList));
    newExprList->len = exprList->len;
    newExprList->capity = exprList->len;
    newExprList->exprs = (JavaExpr **)malloc(sizeof(JavaExpr*)*exprList->len);
    
    
    for (int i = 0; i < exprList->len; i++) {
        
        JavaExpr *oldExpr = copyExpr(*(exprList->exprs+i));
        memcpy((newExprList->exprs + i), &oldExpr, sizeof(JavaExpr *));
        
    }
    
    return exprList;
    
}

JavaClassItems *copyClassItems(JavaClassItems *clsItems){
    
    JavaClassItems *newClsItems = (JavaClassItems *)malloc(sizeof(JavaClassItems));
    newClsItems->functions = clsItems->functions;
    newClsItems->declareVarList = copyExprList(clsItems->declareVarList);
    
    return newClsItems;
}

MetaJavaClass *instanceClass(MetaJavaClass *clss){
    
    MetaJavaClass *cls = (MetaJavaClass *)malloc(sizeof(MetaJavaClass));
    cls->clsName = clss->clsName;
    cls->clsItems = copyClassItems(clss->clsItems);
    if (clss->superClass != 0) {
        cls->superClass = instanceClass(clss->superClass);
    }
    return cls;
};

MetaJavaClass *newInstanceClass(JavaParser *parser, Token *clsToken){
    
    MetaJavaClass *metaCls = findClssWithToken(parser, *clsToken);
    
    if (metaCls == 0) {
        return 0;
    }
    return instanceClass(metaCls);

};


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
                
                if (leftExpr->type == ExprType_VAR && leftExpr->declareType == HC_ID) {
                    JavaExpr *clsInstaceVar = initExpr();
                    clsInstaceVar->type = ExprType_CLSINSTANCE;
                    clsInstaceVar->token = leftExpr->token;
                    clsInstaceVar->varToken = leftExpr->varToken;
                   
                    MetaJavaClass *cls = newInstanceClass(parser, leftExpr->token);
                    if (cls == NULL) {
                        char *errormsg = (char *)malloc(sizeof(char)*(leftExpr->token->n+1));
                        memset(errormsg, '\0', (leftExpr->token->n+1));
                        memcpy(errormsg, leftExpr->token->z, leftExpr->token->n);
                        printf("Undefined clss %s",errormsg);
                        free(errormsg);
                        return 0;
                    }
                    clsInstaceVar->clsInstance = cls;
                    memcpy((tmpVarList->exprs+tmpVarList->len), &clsInstaceVar, sizeof(JavaExpr*));
                    tmpVarList->len++;
                }else if(leftExpr->type == ExprType_THISTOKEN){
                    
                    JavaExpr *existExpr = findClassInstanceVarExpr(cls,leftExpr->varToken);
                    if (rightExpr->type == ExprType_CLSINSTANCE) {
                       JavaExpr *rightVarExpr = findVarExpr(tmpVarList, rightExpr->token, rightExpr->propertyToken);
                       existExpr->valueTokenType = rightVarExpr->valueTokenType;
                       existExpr->valueToken = rightExpr->valueToken;
                    }else if (rightExpr->type == ExprType_VAR){
                        JavaExpr *rightVarExpr = findVarExpr(tmpVarList, 0, rightExpr->varToken);
                        existExpr->valueTokenType = rightVarExpr->valueTokenType;
                        existExpr->valueToken = rightVarExpr->valueToken;
                    }else if (rightExpr->type == ExprType_CONSTANT){
                        existExpr->valueTokenType = rightExpr->tokenType;
                        existExpr->valueToken = rightExpr->token;
                    }else if (rightExpr->type == ExprType_CALL){
                        
                        if (rightExpr->propertyToken != 0) {
                            
                            JavaExpr *clsVar = findVarExpr(tmpVarList, rightExpr->token, 0);
                            JavaFunction *func = findInstanceFunc(clsVar->clsInstance, rightExpr->propertyToken);
                            JavaExpr *returnEx = runFunc(parser, clsVar->clsInstance, rightExpr->callParList, func);
                            
                            existExpr->valueTokenType = returnEx->valueTokenType;
                            existExpr->valueToken = returnEx->valueToken;
                           
                        }
                        
                        
                    }
                    
                }else{
                    
                   JavaExpr *existExpr = findVarExpr(tmpVarList, leftExpr->token, leftExpr->varToken);
                    if (existExpr == 0) {
                        printf("undefine var %s",leftExpr->token->z);
                        return 0;
                    }
                    
                    //类属性
                   if (rightExpr->type == ExprType_CLSINSTANCE) {
                       JavaExpr *rightVarExpr = findVarExpr(tmpVarList, rightExpr->token, rightExpr->propertyToken);
                       existExpr->valueTokenType = rightVarExpr->valueTokenType;
                       existExpr->valueToken = rightExpr->valueToken;
                    }else if (rightExpr->type == ExprType_VAR){
                        JavaExpr *rightVarExpr = findVarExpr(tmpVarList, 0, rightExpr->token);
                        existExpr->valueTokenType = rightVarExpr->valueTokenType;
                        existExpr->valueToken = rightVarExpr->valueToken;
                    }else if (rightExpr->type == ExprType_CONSTANT){
                        existExpr->valueTokenType = rightExpr->tokenType;
                        existExpr->valueToken = rightExpr->token;
    
                    }else if (rightExpr->type == ExprType_CALL){
                        
                        if (rightExpr->propertyToken != 0) {
                            
                            JavaExpr *clsVar = findVarExpr(tmpVarList, rightExpr->token, 0);
                            JavaFunction *func = findInstanceFunc(clsVar->clsInstance, rightExpr->propertyToken);
                            JavaExpr *returnEx = runFunc(parser, clsVar->clsInstance, rightExpr->callParList, func);
                            if (returnEx == 0) {
                                printf("has not return value\n");
                                continue;
                            }
                            existExpr->valueTokenType = returnEx->valueTokenType;
                            existExpr->valueToken = returnEx->valueToken;
                           
                        }
                        
                        
                    }
                }
                
            }
                continue;
            case ExprType_CALL:
            {
                if (expr->propertyToken != 0) {
                    JavaExpr *classVarExpr = findVarExpr(tmpVarList, 0, expr->token);
                    if (classVarExpr == 0) {
                        printf("undefined var %s",expr->token->z);
                        return 0;
                    }
                    JavaFunction *func = findInstanceFunc(classVarExpr->clsInstance, expr->propertyToken);
                    runFunc(parser, classVarExpr->clsInstance, expr->callParList, func);
                }else{
                    
                    if (strncmp("printf", expr->token->z, expr->token->n) == 0) {
                        if (expr->callParList->len != 1) {
                            printf("printf only support 1 argument!\n");
                        }
                        
                        JavaExpr *pExpr = *(expr->callParList->exprs);
                        
                        if (pExpr->type == ExprType_VAR) {
                            
                            if (pExpr->token != 0) {
                                JavaExpr *varEx = findVarExpr(tmpVarList, pExpr->token, pExpr->varToken);
                                if (varEx == 0) {
                                    printf("undefined var %s.%s\n",pExpr->token->z,pExpr->varToken->z);
                                }else if (varEx->valueToken == 0){
                                    printf("can't print %s.%s value,because is null\n",pExpr->token->z,pExpr->varToken->z);
                                }else{
                                     printf("%s\n",varEx->valueToken->z);
                                }
                                
                            }else{
                                JavaExpr *varEx = findVarExpr(tmpVarList, 0, pExpr->varToken);
                                if (varEx != 0) {
                                    printf("%s\n",varEx->varToken->z);
                                }else{
                                    printf("undefined var %s.%s\n",pExpr->token->z,pExpr->varToken->z);
                                }
                            }
                            
                        }else if (pExpr->type == ExprType_CONSTANT){
                            
                            if (pExpr->token != 0) {
                                printf("%s\n",pExpr->token->z);
                            }else{
                                printf("prinf error!\n");
                            }
                              
                        }else if (pExpr->type == ExprType_THISTOKEN){
                            JavaExpr *varEx = findClassInstanceVarExpr(cls, pExpr->varToken);
                            if (varEx != 0) {
                                printf("%s\n",varEx->varToken->z);
                            }else{
                                printf("undefined var %s.%s\n",pExpr->token->z,pExpr->varToken->z);
                            }
                        }
                        
    
                    }
                }
            }
                continue;
            case ExprType_DECLARE:{
             
                if (expr->exprs == 0 || expr->exprLen == 0) {
                    continue;
                }
                
                for (int i = 0; i < expr->exprLen; i++) {
            
                    JavaExpr *localExpr = *(expr->exprs + i);
                    if (localExpr->type == ExprType_TOKEN) {
                        JavaExpr *varExpr = initExpr();
                        varExpr->type = ExprType_VAR;
                        varExpr->declareType = expr->declareType;
                        varExpr->token = localExpr->token;
                        varExpr->tokenType = localExpr->tokenType;
                        memcpy((tmpVarList->exprs+tmpVarList->len), &varExpr, sizeof(JavaExpr*));
                        tmpVarList->len++;
                    }else if(localExpr->type == ExprType_ASSGIN){
                        
                        JavaExpr *leftExpr = localExpr->assginLeftExpr;
                        JavaExpr *rightExpr = localExpr->assginRightExpr;
                    
                        JavaExpr *varExpr = initExpr();
                        varExpr->type = ExprType_VAR;
                        varExpr->declareType = expr->declareType;
                        varExpr->token = leftExpr->token;
                        varExpr->tokenType = leftExpr->tokenType;
                        
                         if (rightExpr->type == ExprType_CLSINSTANCE) {
                             JavaExpr *rightVarExpr = findVarExpr(tmpVarList, rightExpr->token, rightExpr->propertyToken);
                             varExpr->valueTokenType = rightVarExpr->valueTokenType;
                             varExpr->valueToken = rightExpr->valueToken;
                         }else if (rightExpr->type == ExprType_VAR){
                             JavaExpr *rightVarExpr = findVarExpr(tmpVarList, 0, rightExpr->token);
                             varExpr->valueTokenType = rightVarExpr->valueTokenType;
                             varExpr->valueToken = rightVarExpr->valueToken;
                         }else if (rightExpr->type == ExprType_CONSTANT){
                             varExpr->valueTokenType = rightExpr->tokenType;
                             varExpr->valueToken = rightExpr->token;
                         }
                        
                        memcpy((tmpVarList->exprs+tmpVarList->len), &varExpr, sizeof(JavaExpr*));
                        tmpVarList->len++;
                
                    }
                    
            
                }
                
                
            }
                continue;
            case ExprType_RETURN:
            {
                if (expr->returnExpr->type == ExprType_CONSTANT) {
                    expr->valueToken = expr->returnExpr->token;
                    expr->valueTokenType = expr->returnExpr->tokenType;
                } else if(expr->returnExpr->type == ExprType_VAR){
                    JavaExpr *rightVarExpr = findVarExpr(tmpVarList, expr->returnExpr->token, expr->returnExpr->propertyToken);
                    if (rightVarExpr == 0) {
                        printf("undefined var %s\n",expr->returnExpr->varToken->z);
                    }else{
                        expr->valueToken = rightVarExpr->valueToken;
                        expr->valueTokenType = rightVarExpr->valueTokenType;
                    }
                   
                }else if (expr->returnExpr->type == ExprType_CLSINSTANCE){
                    JavaExpr *rightVarExpr = findVarExpr(tmpVarList, expr->returnExpr->token, expr->returnExpr->propertyToken);
                    expr->valueTokenType = rightVarExpr->valueTokenType;
                    expr->valueToken = rightVarExpr->valueToken;
                }

                return expr;
            }
                break;
            default:
            {
                printf("invaild secentence!\n");
            }
                continue;
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


