#include <stdio.h>
#include "tokenize.h"
#include "simple_java.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
void showToken(const char *sourcecode);
void testToken();
int main(int argc, char *argv[]) {
	
	const char *sjfile = "simple.sj";
	FILE *fp = fopen(sjfile, "r");
	struct stat statbuf;
	stat(sjfile,&statbuf);
	long fileSize = statbuf.st_size;
	char *buff = malloc(fileSize);
	char text[1000];
	while((fgets(text,1000,fp))!=NULL){
		strcat(buff,text);
	}

	showToken(buff);
		

}

void showToken(const char *sourcecode){
	
	int tokenType;
	int len = javaGetToken(sourcecode, &tokenType);
	while (tokenType != HC_ILLEGAL) {
			
		printf("token:\t");
		for (int i = 0; i < len; i++) {
			printf("%c",sourcecode[i]);
		}
		printf("\n");
		printf("tokentype:\t");
		printTokenType(tokenType);
				
		sourcecode = &sourcecode[len];
		len = javaGetToken(sourcecode, &tokenType);
		if (len == 0) {
			break;
		}
				
	}
	
	//printf("source code:%s",sourcecode);
	
}

void testToken(){
	
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