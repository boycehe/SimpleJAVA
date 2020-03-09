
#ifndef simple_java_foundation_h
#define simple_java_foundation_h



typedef struct MetaClass MetaClass;
typedef struct JavaFunction JavaFunction;
typedef struct JavaVar JavaVar;
typedef struct JavaExprList  JavaExprList;
typedef struct JavaExpr	JavaExpr;
typedef struct JavaSentenceList JavaSentenceList;
typedef struct MetaJavaClass MetaJavaClass;
typedef struct JavaSentence JavaSentence;
typedef struct JavaClassBody JavaClassBody;
typedef struct JavaParser JavaParser;
typedef struct JavaDeclareVar JavaDeclareVar;
typedef struct JavaParameterlist JavaParameterlist;
typedef struct JavaParameter JavaParameter;
typedef struct MetaJavaClassList MetaJavaClassList;
typedef struct CallTokensList CallTokensList;
typedef struct CallTokens CallTokens;
typedef struct JavaClassItems JavaClassItems;
typedef struct JavaFunctionList JavaFunctionList;
typedef enum ExprType ExprType;
typedef enum JavaAccessType JavaAccessType;


/*
access ::= PUBLIC.
access ::= PROTECTED.
access ::= PRIVATE.
*/

struct Token {
	const unsigned char *z;     /* Text of the token.  Not NULL-terminated! */
	unsigned int n;    /* Number of characters in this token */
};
typedef struct Token Token;

enum JavaAccessType {
	PUBLIC = 0,
	PROTECTED = 1,
	PRIVATE = 2
};

enum ExprType {
    ExprType_ASSGIN = 0,
    ExprType_VAR = 1,
    ExprType_THISTOKEN = 2,
    ExprType_NEW = 3,
    ExprType_RETURN = 4,
    ExprType_DECLARE = 5,
    ExprType_CLSINSTANCE = 6
};

enum JavaType {
	 JAVA_INT = 0,
	 JAVA_STRING
};

struct CallTokensList{
	JavaExpr **exprs;
	unsigned char len;
    unsigned char capity;
};

struct CallTokens{
	Token *token;
	int tokenType;
};

struct JavaClassItems {
    
	JavaFunctionList *functions;
	JavaExprList *declareVarList;
    
};

struct MetaJavaClassList {
	MetaJavaClass **cls;
	unsigned char clsLen;
};

struct MetaJavaClass {
	MetaJavaClass  *superClass;
    char *clsName;
    JavaClassItems *clsItems;
};

struct JavaDeclareVar {
	
};

struct JavaExprList{
	JavaExpr **exprs;
	unsigned short len;
    unsigned short capity;
};

struct JavaParameterlist{
	unsigned short len;
    unsigned short capity;
	JavaParameter **parmeters;
};

struct JavaParameter{
	unsigned type;
	Token *token;
};

struct JavaExpr{
	
    ExprType type;
    int     declareType;
	Token 	*token;
    int      tokenType;
	Token 	*propertyToken;
    Token   *varToken;
    Token   *valueToken;
    int      valueTokenType;
    CallTokensList *callParList;
    JavaExpr *assginLeftExpr;
    JavaExpr *assginRightExpr;
    JavaExpr **exprs;
    JavaExpr *returnExpr;
    MetaJavaClass *clsInstance;
    unsigned char exprLen;
    unsigned char exprCapity;

};

struct JavaFunctionList {
    JavaFunction **funcs;
    unsigned char len;
    unsigned char capity;
};

struct JavaFunction {
    JavaAccessType accessType;
    int returnType;
	const char *name;
	JavaParameterlist *parameters;
	JavaExprList *sentenceList;
};

struct JavaSentenceList {
	
};

struct JavaSentence {
	
};

struct JavaVar {
	
	unsigned char type;

};

struct JavaClassBody {
	JavaFunction *functions;
	unsigned char funLen;
	JavaVar **javaVars;
	unsigned char javaVarLen;
};

struct JavaParser {
	
	 MetaJavaClass **classes;
	 unsigned clsLen;
     JavaFunction *entryFunc;
     JavaClassItems *clsItems;
};

#endif
