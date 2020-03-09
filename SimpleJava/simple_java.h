#include "tokenize.h"
#include "simple_java_foundation.h"

#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif
#define ParseTOKENTYPE Token
#define ParseARG_PDECL ,JavaParser *pParse

void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
void Parse(
			  void *yyp,                   /* The parser */
			  int yymajor,                 /* The major token code number */
			  ParseTOKENTYPE yyminor       /* The value for the token */
			  ParseARG_PDECL               /* Optional %extra_argument parameter */
);
void ParseFree(
					void *p,                    /* The parser to be deleted */
					void (*freeProc)(void*)     /* Function used to reclaim memory */
);
