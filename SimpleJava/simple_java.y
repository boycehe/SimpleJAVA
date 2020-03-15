%token_type {Token}
%default_type {Token}
%extra_argument {JavaParser *pParse}
%token_prefix HC_

%include {
	#include "simple_java.h"
	#include <assert.h>
	#include <stdlib.h>
    #include "semantic.h"
    #include "simple_java_parser.h"
}

%syntax_error {
	char *errSyntaxMsg = (char *)malloc(TOKEN.n);
	memcpy(errSyntaxMsg,(void*)TOKEN.z,TOKEN.n);
	printf("error:near %s happen syntax error！\n",errSyntaxMsg);
	free(errSyntaxMsg);
}

//类声明
%type javabegin {MetaJavaClassList *}
%type javaclass {MetaJavaClassList *}
javabegin ::= javaclass.{

} 

javaclass ::= javaclass CLASS ID(B) javasinherit(C) LD classitems(D) RD.{
	createClass(pParse,B,C,D);
}

javaclass ::= .{
  
}
%type expr {JavaExpr*}
expr(A) ::= ID(B).{
	A = varTokenToExpr(B,@B);
}
expr(A) ::= TEXT(B).{
	A = varTokenToExpr(B,@B);
}
expr(A) ::= INTEGER(B).{
	A = varTokenToExpr(B,@B);
}

expr(A) ::= ID(B) DOT ID(C).{
	A =  instanceGetProperty(@B,B,C);
}
//方法调用 入参类型 text，int 类属性，暂不支持返回值入参
%type callexpr {JavaExpr*}
%type callNoSemi {JavaExpr*}
callexpr(A) ::= callNoSemi(B) SEMI.{
    A = B;
}
callNoSemi(A) ::= ID(B) DOT ID(C) LP callparameterlist(D) RP.{
    A = dressedCallExpr(B,C,D);
}
callexpr(A) ::= ID(B) LP callparameterlist(C) RP SEMI.{
	Token token;
	token.z = 0;
	token.n = 0;
	A = dressedCallExpr(token,B,C);
}
%type callparameterlist {CallTokensList *}
%type callidist {CallTokensList *}
callparameterlist(A) ::= callidist(B) expr(C).{
	A = addTokenToCallParameterList(B,C);
}

callparameterlist(A) ::= .{
	A = 0;
}

callidist(A) ::= callidist(B) INTEGER(C) COMMA.{
	JavaExpr *expr = varTokenToExpr(C,@C);
	A = addTokenToCallParameterList(B,expr);
}

callidist(A) ::= callidist(B) TEXT(C) COMMA.{
	JavaExpr *expr = varTokenToExpr(C,@C);
	A = addTokenToCallParameterList(B,expr);
}
callidist(A) ::= callidist(B) ID(C) DOT ID(D) COMMA.{
	JavaExpr *expr = instanceGetProperty(@C,C,D);
	A = addTokenToCallParameterList(B,expr);
}
callidist(A) ::= .{
	A = 0;
}

//变量声明
%type declarevar {JavaExpr*}
%type declarevaritems {JavaExpr*}
%type declareexpr {JavaExpr*}
declarevar(A) ::= INT|STRING(B) declarevaritems(C) declareexpr(D) SEMI.{
	A = finishDeclareVars(@B,C,D);
}

declareexpr(A) ::= ID(B).{
    A = varTokenToExpr(B,@B);
}

declareexpr(A) ::= ID(B) EQ INTEGER(C).{
    JavaExpr *leftExpr = varTokenToExpr(B,@B);
    JavaExpr *rightExpr = varTokenToExpr(C,@C);
    A = assginmentExpr(leftExpr,rightExpr);
}

declareexpr(A) ::= ID(B) EQ ID(C).{
    JavaExpr *leftExpr = varTokenToExpr(B,@B);
    JavaExpr *rightExpr = varTokenToExpr(C,@C);
    A = assginmentExpr(leftExpr,rightExpr);
}

declareexpr(A) ::= ID(B) EQ TEXT(C).{
    JavaExpr *leftExpr = varTokenToExpr(B,@B);
    JavaExpr *rightExpr = varTokenToExpr(C,@C);
    A = assginmentExpr(leftExpr,rightExpr);
}

declarevaritems(A) ::= declarevaritems(B) ID(C) COMMA.{
	JavaExpr *expr = varTokenToExpr(C,@C);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ INTEGER(D) COMMA.{
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ ID(D) COMMA.{
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ TEXT(D) COMMA.{
    JavaExpr *leftExpr = varTokenToExpr(C,@C);
    JavaExpr *rightExpr = varTokenToExpr(D,@D);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
    A = addDeclarevarItem(B,expr);
}
declarevaritems(A) ::= .{
	A = 0;
}


//类方法
%type function {JavaFunction *}
function(A) ::= PUBLIC|PROTECTED|PRIVATE(B) INT|STRING|VOID(C) ID(D) LP parameterlist(E) RP functionbody(F).{
	A = createFunction(pParse,@B,@C,D,E,F);
}

%type funcbodyitems {JavaExprList *}
%type functionbody  {JavaExprList *}

functionbody(A) ::= LD funcbodyitems(B) RD.{
	A = B;
}

funcbodyitems(A) ::= funcbodyitems(B) declarevar(C).{
	A = addExprToList(B,C);
}

funcbodyitems(A) ::= funcbodyitems(B) assign(C).{
	A = addExprToList(B,C);
}
funcbodyitems(A) ::= funcbodyitems(B) callexpr(C).{
	A = addExprToList(B,C);
}
funcbodyitems(A) ::= funcbodyitems(B) returnval(C).{
	A = addExprToList(B,C);
}


funcbodyitems(A) ::= .{
	A = 0;
}
//赋值
%type leftval {JavaExpr *}
%type rightval {JavaExpr *}
%type assign {JavaExpr *}
assign(A) ::= leftval(B) EQ rightval(C) SEMI.{
	A = assginmentExpr(B,C);
}

leftval(A) ::= ID(B) ID(C).{
	A = declareClassVar(B,C);
}

leftval(A) ::= THIS DOT ID(C).{
	A = getClassProperty(C);
}

leftval(A) ::= ID(B).{
	A = varTokenToExpr(B,@B);
}
leftval(A) ::= ID(B) DOT ID(C).{
	A =  instanceGetProperty(@B,B,C);
}
rightval(A) ::= ID(B).{
	A = varTokenToExpr(B,@B);
}

rightval(A) ::= THIS(B) DOT ID(C).{
	A =  instanceGetProperty(@B,B,C);
}

rightval(A) ::= ID(B) DOT ID(C).{
	A =  instanceGetProperty(@B,B,C);
}

rightval(A) ::= callNoSemi(B).{
    A = B;
}

rightval(A) ::= NEW ID(B) LP RP.{
   A = newClsInstance(B);
}
rightval(A) ::= INTEGER(B).{
	A = varTokenToExpr(B,@B);
}
rightval(A) ::= TEXT(B).{
	A = varTokenToExpr(B,@B);
}
%type returnval {JavaExpr *}
returnval(A) ::= RETURN rightval(B) SEMI.{
	A = returnExpr(B);
}
returnval(A) ::= RETURN SEMI.{
	A = 0;
}
%type parameterlist {JavaParameterlist *}
parameterlist(A) ::= parameterlist(B) INT|STRING|ID(C) ID(D).{
	A = addParameter(B,@C,D);
}
parameterlist(A) ::= .{
	A = 0;
}

javasinherit(A) ::= EXTENDS ID(B).{
	A = B;
}
javasinherit(A) ::= .{
    Token token;
	token.z = 0;
	token.n = 0;
	A = token;
}

%type classitems {JavaClassItems *}
classitems(A) ::= classitems(B) declarevar(C).{
  	A = dressedClassBodyWithVars(B,C);
}

classitems(A) ::= classitems(B) function(C).{
    if(C != 0){
        A = dressedClassBodyWithFunc(B,C);
    }else{
        A = B;
    }
	
}

classitems(A) ::= .{
	A = 0;
}




