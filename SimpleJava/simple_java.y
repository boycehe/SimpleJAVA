%token_type {Token}
%default_type {Token}
%extra_argument {JavaParser *pParse}
%token_prefix HC_

%include {
	#include "simple_java.h"
	#include <assert.h>
	#include <stdlib.h>
    #include "semantic.h"
}


/*
类声明
*/

/*
1.类声明

2.方法声明

3.变量声明
3.1 int变量声明
3.2 String变量声明

4.赋值语句
4.1 int赋值
4.2 string赋值
4.3 类属性赋值
4.4 类初始化

5.计算语句
5.1 int 计算

6.方法调用

7.入口函数执行

**************
1.赋值语句
2.声明语句
3.计算语句
4.调用语句
5.类属性访问
6.返回值
*/

%syntax_error {
	char *errSyntaxMsg = (char *)malloc(TOKEN.n);
	memcpy(errSyntaxMsg,(void*)TOKEN.z,TOKEN.n);
	printf("near %s syntax error！\n",errSyntaxMsg);
	free(errSyntaxMsg);
}

//类声明
%type javabegin {MetaJavaClassList *}
%type javaclass {MetaJavaClassList *}
javabegin ::= javaclass.{
	printf("0000000\n");
} 

javaclass ::= javaclass CLASS ID(B) javasinherit(C) LD classitems(D) RD.{
	printf("rule--->\t1111111\n");
	createClass(pParse,B,C,D);
}

javaclass ::= .{
    printf("javaclass nil\n");
}
%type expr {JavaExpr*}
expr(A) ::= ID(B).{
	printf("rule--->\t333333333\n");
	A = varTokenToExpr(B,@B);
}
expr(A) ::= TEXT(B).{
	printf("rule--->\t4444444444\n");
	A = varTokenToExpr(B,@B);
}
expr(A) ::= INTEGER(B).{
	printf("rule--->\t555555555\n");
	A = varTokenToExpr(B,@B);
}

expr(A) ::= ID(B) DOT ID(C).{
	printf("rule--->\t6666666666\n");
	A =  instanceGetProperty(@B,B,C);
}
//方法调用 入参类型 text，int 类属性，赞不支持返回值入参
%type callexpr {JavaExpr*}
callexpr(A) ::= ID(B) DOT ID(C) LP callparameterlist(D) RP SEMI.{
	printf("rule--->\t777777777777\n");
	A = dressedCallExpr(B,C,D);
}
callexpr(A) ::= ID(B) LP callparameterlist(C) RP SEMI.{
	printf("rule--->\t888888888\n");
	Token token;
	token.z = 0;
	token.n = 0;
	A = dressedCallExpr(token,B,C);
}
%type callparameterlist {CallTokensList *}
%type callidist {CallTokensList *}
callparameterlist(A) ::= callidist(B) expr(C).{
	printf("rule--->\t999999999\n");
	A = addTokenToCallParameterList(B,C);
}

callparameterlist(A) ::= .{
	printf("rule--->\taaaaaaaaaa\n");
	A = 0;
}

callidist(A) ::= callidist(B) INTEGER(C) COMMA.{
	printf("rule--->\tbbbbbbbbb\n");
	JavaExpr *expr = tokenToExpr(C,@C);
	A = addTokenToCallParameterList(B,expr);
}

callidist(A) ::= callidist(B) TEXT(C) COMMA.{
	printf("rule--->\tccccccccccc\n");
	JavaExpr *expr = tokenToExpr(C,@C);
	A = addTokenToCallParameterList(B,expr);
}
callidist(A) ::= callidist(B) ID(C) DOT ID(D) COMMA.{
	printf("rule--->\tddddddddddd\n");
	JavaExpr *expr = instanceGetProperty(@C,C,D);
	A = addTokenToCallParameterList(B,expr);
}
callidist(A) ::= .{
	printf("rule--->\teeeeeeeeeeee\n");
	A = 0;
}

//变量声明
%type declarevar {JavaExpr*}
%type declarevaritems {JavaExpr*}
declarevar(A) ::= INT|STRING(B) declarevaritems(C) ID(D) SEMI.{
	printf("rule--->\tfffffffffff\n");
	A = finishDeclareVars(@B,C,D,@D);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) COMMA.{
	printf("rule--->\tggggggggg\n");
	JavaExpr *expr = varTokenToExpr(C,@C);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ INTEGER(D) COMMA.{
	printf("rule--->\thhhhhhhhh\n");
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ ID(D) COMMA.{
	printf("rule--->\tiiiiiiiiiiiii\n");
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ TEXT(D) COMMA.{
	printf("rule--->\tjjjjjjjjjjjjj\n");
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
    A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= .{
	printf("rule--->\tkkkkkkkkkk\n");
	A = 0;
}


//类方法
%type function {JavaFunction *}
function(A) ::= PUBLIC|PROTECTED|PRIVATE(B) INT|STRING|VOID(C) ID(D) LP parameterlist(E) RP functionbody(F).{
	printf("rule--->\tlllllllllllll\n");
	A = createFunction(pParse,@B,@C,D,E,F);
}

%type funcbodyitems {JavaExprList *}
%type functionbody  {JavaExprList *}

functionbody(A) ::= LD funcbodyitems(B) RD.{
	printf("rule--->\tmmmmmmmmmmmmm\n");
	A = B;
}

funcbodyitems(A) ::= funcbodyitems(B) declarevar(C).{
	printf("rule--->\tnnnnnnnnnnnn\n");
	A = addExprToList(B,C);
}

funcbodyitems(A) ::= funcbodyitems(B) assign(C).{
	printf("rule--->\tooooooooooooooo\n");
	A = addExprToList(B,C);
}
funcbodyitems(A) ::= funcbodyitems(B) callexpr(C).{
	printf("rule--->\tnnnnnnnnnnnnn\n");
	A = addExprToList(B,C);
}
funcbodyitems(A) ::= funcbodyitems(B) returnval(C).{
	printf("rule--->\tpppppppppppppp\n");
	A = addExprToList(B,C);
}


funcbodyitems(A) ::= .{
	printf("rule--->\tqqqqqqqqqq\n");
	A = 0;
}
//赋值
%type leftval {JavaExpr *}
%type rightval {JavaExpr *}
%type assign {JavaExpr *}
assign(A) ::= leftval(B) EQ rightval(C) SEMI.{
	printf("rule--->\trrrrrrrrrrrrr\n");
	A = assginmentExpr(B,C);
}

leftval(A) ::= ID(B) ID(C).{
	printf("rule--->\tsssssssssssssss\n");
	A = declareClassVar(B,C);
}

leftval(A) ::= THIS DOT ID(C).{
	printf("rule--->\tttttttttttttttttt\n");
	A = getClassProperty(C);
}

leftval(A) ::= ID(B).{
	printf("rule--->\tuuuuuuuuuuuuuu\n");
	A = tokenToExpr(B,@B);
}
leftval(A) ::= ID(B) DOT ID(C).{
	printf("rule--->\tvvvvvvvvvvvvvvvvvv\n");
	A =  instanceGetProperty(@B,B,C);
}
rightval(A) ::= ID(B).{
	printf("rule--->\twwwwwwwwwwwwwww\n");
	A = tokenToExpr(B,@B);
}

rightval(A) ::= THIS(B) DOT ID(C).{
	printf("rule--->\txxxxxxxxxxxxxxxx\n");
	A =  instanceGetProperty(@B,B,C);
}

rightval(A) ::= NEW ID(B) LP RP.{
	printf("rule--->\tyyyyyyyyyyyyyy\n");
   A = newClsInstance(B);
}
rightval(A) ::= INTEGER(B).{
	printf("rule--->\tzzzzzzzzzzzzzzzzzzz\n");
	A = varTokenToExpr(B,@B);
}
rightval(A) ::= TEXT(B).{
	printf("rule--->\tAAAAAAAAAAAAAAA\n");
	A = varTokenToExpr(B,@B);
}
%type returnval {JavaExpr *}
returnval(A) ::= RETURN rightval(B) SEMI.{
	printf("rule--->\tBBBBBBBBBBBBBBBB\n");
	A = returnExpr(B);
}
returnval(A) ::= RETURN SEMI.{
	printf("rule--->\tCCCCCCCCCCCCCCCCCC\n");
	A = 0;
}
%type parameterlist {JavaParameterlist *}
parameterlist(A) ::= parameterlist(B) INT|STRING|ID(C) ID(D).{
	printf("rule--->\tDDDDDDDDDDDDDDDDDDDD\n");
	A = addParameter(B,@C,D);
}
parameterlist(A) ::= .{
	printf("rule--->\tEEEEEEEEEEEEEEEEE\n");
	A = 0;
}

javasinherit(A) ::= EXTENDS ID(B).{
	printf("rule--->\tFFFFFFFFFFFFFFFFF\n");
	A = B;
}
javasinherit(A) ::= .{
	printf("rule--->\tGGGGGGGGGGGGGG\n");
    Token token;
	token.z = 0;
	token.n = 0;
	A = token;
}

%type classitems {JavaClassItems *}
classitems(A) ::= classitems(B) declarevar(C).{
	printf("rule--->\tHHHHHHHHHHHHHH\n");
  	A = dressedClassBodyWithVars(B,C);
}

classitems(A) ::= classitems(B) function(C).{
	printf("rule--->\tIIIIIIIIIIII\n");
    if(C != 0){
        A = dressedClassBodyWithFunc(B,C);
    }else{
        A = B;
    }
	
}

classitems(A) ::= .{
	printf("rule--->\tJJJJJJJJJJJJJJJ\n");
	A = 0;
}




