//
//  dogger_tokenize.h
//  Dogger
//
//  Created by heboyce on 2019/10/9
//  Copyright © 2019 heboyce. All rights reserved.
//

/*
class
new 
int
String
public
private
protected
return
extends
void
*/



#define HC_EQ                              1
#define HC_CLASS                           2
#define HC_NEW                             3
#define HC_INT                             4
#define HC_STRING                          5
#define HC_PUBLIC                          6
#define HC_PRIVATE                         7
#define HC_PROTECTED                       8
#define HC_RETURN                          9
#define HC_EXTENDS                         10
#define HC_VOID                            11
#define HC_COMMA                           12
#define HC_LP                              13
#define HC_RP                              14
#define HC_TEXT                            15
#define HC_INTEGER                         16
#define HC_DOT                             17
#define HC_SPACE                           18
#define HC_ILLEGAL                         19
#define HC_SEMI							   20
#define HC_LD							   21
#define HC_RD							   22 
#define HC_ID							   23
#define HC_FLOAT						   24 
#define HC_PLUS						  	   25 
#define HC_MINUS						   26 
#define HC_DIVIDE						   27 
#define HC_MULTI						   28 



int javaGetToken(const unsigned char *z, int *tokenType);
void printTokenType(int tokenType);
void testKeywordCode(const char *z, int n);
