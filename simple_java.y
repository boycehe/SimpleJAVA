%token_type {Token}
%default_type {Token}
%extra_argument {JavaParser *pParse}
%token_prefix HC_

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

//类声明
javaclass ::= CLASS ID(A) javasinherit(B) LD classitems(C) RD.{
	//createClass(pParse,A,B,C);
	printf("275\n");
}

//基础语句
expr(A) ::= LP expr(B) RP.{
	//A = B;
	printf("52\n");
}
expr(A) ::= TEXT(B).{
	//A = B;
	printf("53\n");
}
expr(A) ::= INTEGER(B).{
	//A = B;
	printf("54\n");
}

expr(A) ::= ID(B) DOT ID(C).{
	printf("55\n");
}
//四则运算
opcal(A) ::= opcal(B) INTEGER(C) PLUS|MULTI|DIVIDED|MINUS(OP).{
	printf("91\n");
}
opcal(A) ::= .{
	printf("95\n");
}

sentence(A) ::= opcal(B) INTEGER(C).{
	printf("107\n");
}


//方法调用 入参类型 text，int 类属性，赞不支持返回值入参
callexpr(A) ::= ID(B) DOT ID(C) LP callparameterlist(D) RP SEMI.{
	printf("116\n");
}
callparameterlist(A) ::= callidist(B) expr(C).{
	printf("119\n");
}
callparameterlist(A) ::= .{
	printf("123\n");
}

callidist(A) ::= callidist(B) INTEGER(C) COMMA.{
	printf("127\n");
}
callidist(A) ::= callidist(B) TEXT(C) COMMA.{
	printf("130\n");
}
callidist(A) ::= callidist(B) ID(C) DOT ID(D) COMMA.{
	printf("133\n");
}
callidist(A) ::= .{
	//A = 0;
	printf("137\n");
}

//变量声明
declarevar(A) ::= INT|STRING(B) declarevaritems SEMI.{
	printf("142\n");
}
declarevaritems(A) ::= declarevaritems(B) ID(C) COMMA.{
	printf("149\n");
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ INTEGER(D) COMMA.{
	printf("152\n");
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ ID(D) COMMA.{
	printf("155\n");
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ TEXT(D) COMMA.{
	printf("155\n");
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ NEW ID(D) LP callparameterlist(E) RP COMMA.{
	printf("158\n");
}

declarevaritems(A) ::= .{
	//A = 0;
	printf("163\n");
}

//赋值
assign ::= leftval(B) EQ rightval(C).{
	//assignExpr(B,C);
	printf("169\n");
}

leftval ::= ID(B).{
	//A = B;
	printf("174\n");
}
leftval(A) ::= ID(B) DOT ID(C).{
	//A =  getProperty(B,C);
	printf("178\n");
}
rightval(A) ::= ID(B).{
	//A = B;
	printf("182\n");
}
rightval ::= NEW ID(B) LP callparameterlist(C) RP.{
   //A = newInit(B,C);
	printf("186\n");
}
rightval(A) ::= INTEGER(B).{
	//A = B;
	printf("190\n");
}
rightval(A) ::= STRING(B).{
	printf("193\n");
}

returnval(A) ::= RETURN rightval(B) COMMA.{
	printf("199\n");
}
returnval(A) ::= RETURN COMMA.{
	printf("206\n");
}

//类方法
function(A) ::= PUBLIC|PROTECTED|PRIVATE(B) INT|STRING|VOID(C) ID(D) LP parameterlist(E) RP functionbody(F).{
	//A = addFunctionToClass(pParse,B,C,D,E,F);
	printf("213\n");
}

function(A) ::= returntype(C) ID(D) LP parameterlist(E) RP functionbody(F).{
	//A = addFunctionToClass(pParse,B,C,D,E,F);
	printf("213\n");
}

functionbody(A) ::= LD funcbodyitems(B) RD.{
	//A = B;
	printf("228\n");
}

funcbodyitems(A) ::= funcbodyitems(B) declarevar(C).{
	printf("232\n");
}

funcbodyitems(A) ::= funcbodyitems assign.{
	printf("236\n");
}
funcbodyitems(A) ::= funcbodyitems callexpr.{
	printf("239\n");
}
funcbodyitems(A) ::= funcbodyitems returnval.{
	printf("240\n");
}

funcbodyitems(A) ::= funcbodyitems sentence.{
	printf("250\n");
}

funcbodyitems(A) ::= .{
	//A = 0;
	printf("243\n");
}

parameterlist(A) ::= parameterlist(B) INT|STRING(C) ID(D).{
	printf("265\n");
}
parameterlist(A) ::= .{
	//A = 0.
	printf("269\n");
}

javasinherit(A) ::= EXTENDS ID(B).{
	//A = B;
	printf("280\n");
}
javasinherit(A) ::= .{
    //A = 0;
	printf("284\n");
}

classitems(A) ::= classitems(B) declarevar(C).{
    //	A = dressedClassBodyWithVars(B,C);
	printf("289\n");
}

classitems(A) ::= classitems(B) function(C).{
//	A = dressedClassBodyWithFunc(B,C);
	printf("294\n");
}

classitems(A) ::= .{
//	A = 0.
	printf("299\n");
}




