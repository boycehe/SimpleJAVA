
#ifndef simple_java_foundation_h
#define simple_java_foundation_h

typedef struct MetaClass MetaClass;
typedef struct JavaFunction JavaFunction;
typedef struct JavaVar JavaVar;

struct MetaJavaClass {
	
	MetaClass *superClass;
	JavaFunction **functions;
	JavaVar **vars;
	
};

struct JavaFunction {
	const char *name;
	JavaVar **parameters;
	unsigned char parametersLen;
};

struct JavaVar {
	
	unsigned char type;
	enum {
		java_int = 0,
		java_string
	}
	
};

#endif