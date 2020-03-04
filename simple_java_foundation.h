
#ifndef simple_java_foundation_h
#define simple_java_foundation_h

typedef struct MetaClass MetaClass;
typedef struct JavaFunction JavaFunction;
typedef struct JavaVar JavaVar;
typedef struct JavaExprList  JavaExprList;
typedef struct JavaExpr	JavaExpr;

/*
access ::= PUBLIC.
access ::= PROTECTED.
access ::= PRIVATE.
*/

enum JavaAccessType {
	PUBLIC = 0,
	PROTECTED = 1,
	PRIVATE = 2
}

enum JavaType {
	 JAVA_INT = 0,
	 JAVA_STRING
}

struct MetaJavaClass {
	
	MetaClass *superClass;
	JavaFunction **functions;
	JavaVar **vars;
	
};

struct JavaExprList{
	JavaExpr **exprs;
	unsigned short len;
}

struct JavaExpr{
	
    unsigned char type;
	Token 	*token;
	Token 	*propertyToken;
	JavaVar *var;
	
}

struct JavaFunction {
	const char *name;
	JavaExprList *parameters;
	JavaSentenceList *sentenceList;
};

struct JavaSentenceList {
	
}

struct JavaSentence {
	
}

struct JavaVar {
	
	unsigned char type;
	enum {
		java_int = 0,
		java_string
	}
	
};

struct JavaClassBody {
	JavaFunction *functions;
	unsigned char funLen;
	JavaVar **javaVars;
	unsigned char javaVarLen;
}

struct JavaParser {
	
	 MetaJavaClass **classes;
	 unsigned clsLen;
	
}

#endif