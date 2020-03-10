#include <stdio.h>
#include "tokenize.h"
#include "simple_java.h"
#include "semantic.h"
#include "simple_java_foundation.h"
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
void showToken(const unsigned char *sourcecode);
void testToken();
int main(int argc, char *argv[]) {
	
	const char *sjfile = "/Users/heboyce/Desktop/Study/LearnCompilers/SimpleJAVA/SimpleJava/simple.sj";
	FILE *fp = fopen(sjfile, "r");
    if (fp==0) {
        printf("can't read file\n");
        return 0;
    }
	struct stat statbuf;
	stat(sjfile,&statbuf);
	long fileSize = statbuf.st_size;
	char *buff = malloc(fileSize);
	char text[1000];
	while((fgets(text,1000,fp))!=NULL){
		strcat(buff,text);
	}
	
	showToken((const unsigned char*)buff);
		
}

void showToken(const unsigned char *sourcecode){
	
	void* pParser = ParseAlloc(malloc);
    JavaParser *pParse = (JavaParser *)malloc(sizeof(JavaParser));
    pParse->entryFunc  = 0;
    pParse->clsCapity  = 10;
    pParse->classes    = (MetaJavaClass **)malloc(sizeof(MetaJavaClass *)*10);
    pParse->clsLen     = 0;
	int tokenType;
	int len = javaGetToken(sourcecode, &tokenType);
	while (tokenType != HC_ILLEGAL) {
			
		Token token ;
        char *z = (char*)malloc(len+1);
        memset(z, '\0', len+1);
		token.z = memcpy(z, sourcecode, len);
		token.n = len;
		
		if (tokenType != HC_SPACE) {
		 	Parse(pParser,tokenType,token,pParse);
			printf("token:\t");
			for (int i = 0; i < len; i++) {
				printf("%c",sourcecode[i]);
			}
			printf("\n");
			printf("tokentype:\t");
			printTokenType(tokenType);
		}

		sourcecode = &sourcecode[len];
		len = javaGetToken(sourcecode, &tokenType);
		if (len == 0) {
			break;
		}
				
	}
	
	Token endToken;
	endToken.z = 0;
	endToken.n = 0;
	Parse(pParser,0,endToken,pParse);
	ParseFree(pParser, free);
    javaRun(pParse);
    
    
    

}

void testToken(){
	
	const char *testql = "class new int String public private protected return extends void this";
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
		const char *token11 = "this";
		testKeywordCode(token11,4);
}
