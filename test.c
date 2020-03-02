#include <stdio.h>
#include "tokenize.h"

int main(int argc, char *argv[]) {
	
	const char *testql = "class new int String public private protected return extends void";
	int tokenType;
	int len = javaGetToken(testql, &tokenType);
	while (tokenType != HC_ILLEGAL) {
		
	    for (int i = 0; i < len; i++) {
		    printf("%c",testql[i]);
		}
		printf("\n");
		printTokenType(tokenType);
		
		testql = &testql[len];
		len = javaGetToken(testql, &tokenType);
		if (len == 0) {
			break;
		}
		
	}
	
	const char *token1 = "class";
	testKeywordCode(token1,5);
	const char *token2 = "new";
	testKeywordCode(token2,3);
	const char *token3 = "int";
	testKeywordCode(token3,3);
	const char *token4 = "String";
	testKeywordCode(token4,6);
	const char *token5 = "public";
	testKeywordCode(token5,6);
	const char *token6 = "private";
	testKeywordCode(token6,7);
	const char *token7 = "protected";
	testKeywordCode(token7,9);
	const char *token8 = "return";
	testKeywordCode(token8,6);
	const char *token9 = "extends";
	testKeywordCode(token9,7);
	const char *token10 = "void";
	testKeywordCode(token10,4);
}