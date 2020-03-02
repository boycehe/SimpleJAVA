%token_type {Token}
%default_type {Token}
%extra_argument {DoggerParse *pParse}

/*
类声明
*/

//方法调用
call ::= ID DOT ID LP callidlist RP SEMI.
callidlist ::= idlist ID COMMA.
callidlist ::= .
//变量声明
declarevar ::= type declarevaritems SEMI.
declarevar ::= .
declarevaritems ::= declarevaritems ID.
declarevaritems ::= declarevaritems INT EQ INT.
declarevaritems ::= declarevaritems STRING EQ STRING.
declarevaritems ::= declarevaritems ID EQ NEW ID LP RP.


//变量声明（多行）
declarevarlist ::= declarevarlist declarevar.
declarevarlist ::= .

//赋值
assign ::= leftval EQ rightval.

leftval ::= ID.
leftval ::= ID DOT ID
rightval ::= ID.
rightval ::= NEW ID LP RP.
rightval ::= INTEGER.
rightval ::= STRING.
//类方法
function ::= access returntype ID functionbody.

returntype ::= type.
returntype ::= .

functionbody =  LP parameterlist RP LD funcbodyitems RD


javaclass ::= CLASS ID javasinherit LD  RD

javasinherit ::= extends ID.
javasinherit ::= .

funcbodyitems ::= funcbodyitems function.
funcbodyitems ::= funcbodyitems declarevar.
funcbodyitems ::= funcbodyitems assign.
funcbodyitems ::= .

access ::= PUBLIC.
access ::= DEFAULT.
access ::= PROTECTED.
access ::= PRIVATE.
access ::= .

parameterlist ::= parameterlist type ID.
parameterlist ::= .

idlist = idlist INT.
idlist = idlist STRING.
idlist = idlist ID DOT ID.
idlist = .

entrymainfunc ::= PUBLIC STATIC VOID MAIN functionbody.

type ::= INT.
type ::= STRING.



