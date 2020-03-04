%token_type {Token}
%default_type {Token}
%extra_argument {JavaParser *pParse}

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

%type vartype {int}
vartype(A) ::= INT(B).{
	A = 0;
}
vartype(A) ::= STRING(B).{
	A = 1;
}

//基础语句
expr(A) ::= LP expr(B) RP.{
	A = B;
}
expr(A) ::= TEXT(B).{
	A = B;
}
expr(A) ::= INTEGER(B).{
	A = B;
}

expr(A) ::= ID(B) DOT ID(C).{
	
}
expr ::= expr(A) EQ expr(B).{
	
}

//四则运算
%type optype {int}
optype(A) ::= PLUS(B).{
	A = @B;
}
optype(A) ::= MULTI(B).{
	A = @B;
}
optype(A) ::= DIVVIDED(B).{
	A = @B;
}
optype(A) ::= MINUS(B).{
	A = @B;
}

opcal(A) ::= intplus(B) INT(C) optype(OP).{
	
}
opcal(A) ::= .{
	A = 0;
}

strplus(A) ::= strplus(B) TEXT(C) PLUS(OP).{
	
}
strplus(A) ::= .{
	 A = 0;
}

expr(A) ::= opcal(B) INT(C) optype(OP) INT(D).{
	
}

expr(A) ::= strplus(B) TEXT(C) PLUS TEXT(D).{
	
}

//方法调用 入参类型 text，int 类属性，赞不支持返回值入参
callexpr(A) ::= ID(B) DOT ID(C) LP callparameterlist(D) RP SEMI.{
	
}
callparameterlist(A) ::= callidist(B) expr(C).{
	
}
callparameterlist(A) ::= .{
	A = 0;
}

callidist(A) = callidist(B) INT(C) COMMA.{
	
}
callidist(A) = callidist(B) STRING(C) COMMA.{
	
}
callidist(A) = callidist(B) ID(C) DOT ID(D) COMMA.{
	
}
callidist(A) = .{
	A = 0;
}

//变量声明
declarevar(A) ::= vartype declarevaritems SEMI.{
	
}
declarevar(A) ::= .{
	A = 0;
}
declarevaritems(A) ::= declarevaritems(B) ID(C).{
	
}
declarevaritems(A) ::= declarevaritems(B) INT(C) EQ INT(D).{
	
}
declarevaritems(A) ::= declarevaritems(B) STRING(C) EQ STRING(D).{
	
}
declarevaritems(A) ::= declarevaritems(B) ID(C) EQ NEW ID(D) LP callparameterlist(E) RP.{
	
}

declarevaritems(A) ::= .{
	A = 0;
}

//赋值
assign ::= leftval(B) EQ rightval(C).{
	assignExpr(B,C);
}

leftval ::= ID(B).{
	A = B;
}
leftval(A) ::= ID(B) DOT ID(C).{
	A =  getProperty(B,C);
}
rightval(A) ::= ID(B).{
	A = B;
}
rightval ::= NEW ID(B) LP callparameterlist(C) RP.{
   A = newInit(B,C);
}
rightval(A) ::= INTEGER(B).{
	A = B;
}
rightval(A) ::= STRING.{
	A = B;
}

returnval(A) ::= RETURN rightval(B).{
	A = B;
}
returnval(A) ::= .{
	A = 0;
}
returnval(A) ::= RETURN .{
	A = 0;
}

//类方法
function(A) ::= access(B) returntype(C) ID(D) LP parameterlist(E) RP functionbody(F).{
	A = addFunctionToClass(pParse,B,C,D,E,F);
}

%type returntype {int}
returntype(A) ::= vartype(B).{
	A = B;
}
returntype(A) ::= VOID.{
	A = 0;
}

functionbody(A) = LD funcbodyitems(B) RD.{
	A = B;
}

funcbodyitems(A) ::= funcbodyitems(B) declarevar(C).{
	
}

funcbodyitems(A) ::= funcbodyitems assign.{
	
}
funcbodyitems(A) ::= funcbodyitems callexpr.{
	
}
funcbodyitems(A) ::= .{
	A = 0;
}

%type access {int}
access(A) ::= PUBLIC.{
	A = 0;
}
access ::= PROTECTED.{
	A = 1;
}
access ::= PRIVATE.{
	A = 2;
}
access ::= .{
	A = 0.
}

parameterlist(A) ::= parameterlist(B) vartype(C) ID(D).{
	
}
parameterlist(A) ::= .{
	A = 0.
}

//类声明
javaclass ::= CLASS ID(A) javasinherit(B) LD classitems(C) RD.{
	createClass(pParse,A,B,C);
}

javasinherit(A) ::= extends ID(B).{
	A = B;
}
javasinherit(A) ::= .{
	A = 0;
}

classitems(A) ::= classitems(B) declarevar(C).{
	A = dressedClassBodyWithVars(B,C);
}

classitems(A) ::= classitems(B) function(C).{
	A = dressedClassBodyWithFunc(B,C);
}

classitems(A) ::= .{
	A = 0.
}




