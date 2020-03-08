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
#define HC_TEXT                             5
#define HC_INTEGER                          6
#define HC_DOT                              7
#define HC_LP                               8
#define HC_RP                               9
#define HC_SEMI                            10
#define HC_COMMA                           11
#define HC_INT                             12
#define HC_STRING                          13
#define HC_EQ                              14
#define HC_NEW                             15
#define HC_PUBLIC                          16
#define HC_PROTECTED                       17
#define HC_PRIVATE                         18
#define HC_VOID                            19
#define HC_THIS                            20
#define HC_RETURN                          21
#define HC_EXTENDS                         22

#define HC_SPACE                           27
#define HC_ILLEGAL                         28
#define HC_FLOAT						   29


int javaGetToken(const unsigned char *z, int *tokenType);
void printTokenType(int tokenType);
void testKeywordCode(const char *z, int n);
