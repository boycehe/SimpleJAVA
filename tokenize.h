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

#define HC_CLASS                            1
#define HC_ID                               2
#define HC_LD                               3
#define HC_RD                               4
#define HC_LP                               5
#define HC_RP                               6
#define HC_TEXT                             7
#define HC_INTEGER                          8
#define HC_DOT                              9
#define HC_PLUS                            10
#define HC_MULTI                           11
#define HC_DIVIDE                          12
#define HC_MINUS                           13
#define HC_SEMI                            14
#define HC_COMMA                           15
#define HC_INT                             16
#define HC_STRING                          17
#define HC_EQ                              18
#define HC_NEW                             19
#define HC_RETURN                          20
#define HC_PUBLIC                          21
#define HC_PROTECTED                       22
#define HC_PRIVATE                         23
#define HC_VOID                            24
#define HC_EXTENDS                         25
#define HC_SPACE                           26
#define HC_ILLEGAL                         27
#define HC_FLOAT						   28
#define HC_THIS						   	   29	




int javaGetToken(const unsigned char *z, int *tokenType);
void printTokenType(int tokenType);
void testKeywordCode(const char *z, int n);
