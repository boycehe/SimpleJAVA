//
//  tokenize.c
//  java
//
//  Created by heboyce on 2019/10/9.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include <stdio.h>
#include "tokenize.h"


#define CC_KYWD       0     /* Alphabetics or '_'.  Usable in a keyword */
#define CC_ID         1     /* unicode characters usable in IDs */
#define CC_DIGIT      2     /* Digits */
#define CC_SPACE      3     /* Space characters */
#define CC_QUOTE      4     /* '"', '\'', or '`'.  String literals, quoted ids */
#define CC_EQ         5     /* '='.  Part of = or == */
#define CC_DOT        6     /* '.' */
#define CC_SEMI       7     /* ';' */
#define CC_COMMA      8     /* ',' */
#define CC_LD         9     /* '{' */
#define CC_RD         10    /* '}' */
#define CC_LP         11    /* '(' */
#define CC_RP         12    /* ')' */
#define CC_PLUS       13    /* '+' */
#define CC_DIVIDE     14    /* '/' */
#define CC_MINUS      15    /* '-' */
#define CC_MULTI      16    /* '*' */
#define CC_ILLEGAL    17    /* Illegal character */
#define CC_NUL        18    /* 0x00 */

#define IdChar(C)  ((javaCtypeMap[(unsigned char)C]&0x46)!=0)
#define javaToupper(x)  ((x)&~(javaCtypeMap[(unsigned char)(x)]&0x20))
#define javaIsspace(x)   (javaCtypeMap[(unsigned char)(x)]&0x01)
#define javaIsalnum(x)   (javaCtypeMap[(unsigned char)(x)]&0x06)
#define javaIsalpha(x)   (javaCtypeMap[(unsigned char)(x)]&0x02)
#define javaIsdigit(x)   (javaCtypeMap[(unsigned char)(x)]&0x04)
#define javaIsxdigit(x)  (javaCtypeMap[(unsigned char)(x)]&0x08)
#define javaTolower(x)   (javaUpperToLower[(unsigned char)(x)])
#define javaIsquote(x)   (javaCtypeMap[(unsigned char)(x)]&0x80)

#define charMap(X) javaUpperToLower[(unsigned char)X]




const unsigned char javaUpperToLower[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
};

const unsigned char javaCtypeMap[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 00..07    ........ */
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,  /* 08..0f    ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 10..17    ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 18..1f    ........ */
    0x01, 0x00, 0x80, 0x00, 0x40, 0x00, 0x00, 0x80,  /* 20..27     !"#$%&' */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 28..2f    ()*+,-./ */
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,  /* 30..37    01234567 */
    0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 38..3f    89:;<=>? */
    
    0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x02,  /* 40..47    @ABCDEFG */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 48..4f    HIJKLMNO */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 50..57    PQRSTUVW */
    0x02, 0x02, 0x02, 0x80, 0x00, 0x00, 0x00, 0x40,  /* 58..5f    XYZ[\]^_ */
    0x80, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x22,  /* 60..67    `abcdefg */
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 68..6f    hijklmno */
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 70..77    pqrstuvw */
    0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 78..7f    xyz{|}~. */
    
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 80..87    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 88..8f    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 90..97    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 98..9f    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a0..a7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a8..af    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b0..b7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b8..bf    ........ */
    
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c0..c7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c8..cf    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d0..d7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d8..df    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e0..e7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e8..ef    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* f0..f7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40   /* f8..ff    ........ */
};

//请使用宽屏查阅该数组
static const unsigned char aiClass[] = {
    /*         x0          x1          x2          x3          x4          x5          x6          x7          x8           x9          xa          xb           xc         xd           xe          xf */
    /* 0x */   CC_NUL    , CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL,  CC_SPACE  , CC_SPACE  , CC_ILLEGAL,  CC_SPACE  , CC_SPACE  , CC_ILLEGAL, CC_ILLEGAL,
    /* 1x */   CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL,  CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL,  CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL,
    /* 2x */   CC_SPACE  , CC_DIGIT  , CC_QUOTE  , CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_QUOTE  , CC_LP     ,  CC_RP     , CC_MULTI  , CC_PLUS   ,  CC_COMMA  , CC_MINUS  , CC_DOT    , CC_DIVIDE ,
    /* 3x */   CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  , CC_DIGIT  ,  CC_DIGIT  , CC_ILLEGAL, CC_SEMI   ,  CC_ILLEGAL, CC_EQ     , CC_ILLEGAL, CC_ILLEGAL,
    /* 4x */   CC_ILLEGAL, CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,
    /* 5x */   CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_ILLEGAL,  CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL, CC_ILLEGAL,
    /* 6x */   CC_ILLEGAL, CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,
    /* 7x */   CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   , CC_KYWD   ,  CC_KYWD   , CC_KYWD   , CC_LD     ,  CC_ILLEGAL, CC_RD     , CC_ILLEGAL, CC_ILLEGAL,
    /* 8x */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* 9x */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Ax */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Bx */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Cx */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Dx */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Ex */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
    /* Fx */   CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     ,  CC_ID     , CC_ID     , CC_ID     , CC_ID     ,
};

static const unsigned char aKWHash[127] = {
  /*       1   2     3     4    5     6     7    8     9    10   11   12   13*/
  /* 1*/  0  ,  3  , 0  ,  0  , 0  ,  7  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 12*/
  /* 2*/  0  ,  10  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 25*/
  /* 3*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 38*/
  /* 4*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 51*/
  /* 5*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 64*/
  /* 6*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 8  ,  0  ,   0  ,    /* 77*/
  /* 7*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  0  ,   0  ,    /* 90*/
  /* 8*/  0  ,  0  , 0  ,  0  , 1  ,  0  ,  0  , 0  ,  0  , 0  , 0  ,  6  ,   0  ,    /* 103*/
  /* 9*/  0  ,  5  , 0  ,  0  , 0  ,  0  ,  0  , 0  ,  4  , 9  , 0  ,  0  ,   0  ,    /* 116*/
  /*10*/  0  ,  0  , 0  ,  0  , 0  ,  0  ,  0  , 2  ,  0  , 0  ,                      /* 126*/
};

static const unsigned char aKWNext[7] = {
    0,   0,   0,   0,   0,   0,   0,
};

static const unsigned char aKWLen[11] = {
    5,   3,   3,   6,
    6,   7,   9,   6,
    7,   4,   4
};
//HC_TASK,HC_DATA,HC_CHART,HC_OR,HC_AND,HC_BEGIN,HC_END
/* aKWOffset[i] is the index into zKWText[] of the start of
 ** the text for the i-th keyword. */
static const unsigned short int aKWOffset[11] = {
    0,   5,    8,  11,  
    17,  23,  30,  39,
    45,  52,  56
};
//HC_TASK,HC_DATA,HC_CHART,HC_OR,HC_AND,HC_BEGIN,HC_END
static const unsigned char aKWCode[11] = {
    HC_CLASS,         HC_NEW,         HC_INT,          HC_STRING,
    HC_PUBLIC,        HC_PRIVATE,     HC_PROTECTED,    HC_RETURN, 
    HC_EXTENDS,       HC_VOID,        HC_THIS
};

/* Hash score: 214 */
/* zKWText[] encodes 950 bytes of keyword text in 50 bytes */
/*   task data chart or and begin end       */

static const char zKWText[60] = {
    'C','L','A','S','S','N','E','W','I','N','T','S','T','R','I','N','G','P','U','B','L','I','C','P','R','I','V',
    'A','T','E','P','R','O','T','E','C','T','E','D','R','E','T','U','R','N','E','X','T','E','N','D','S','V','O',
    'I','D','T','H','I','S'
};
int keywordCode(const char *z, int n, int *pType);
int javaGetToken(const unsigned char *z, int *tokenType){
    int i, c;
    switch( aiClass[*z] ){  /* Switch on the character-class of the first byte
                             ** of the token. See the comment on the CC_ defines
                             ** above. */
        case CC_DOT:{
            *tokenType = HC_DOT;
            return 1;
        }
        case CC_SEMI:{
            *tokenType = HC_SEMI;
            return 1;
        }
        case CC_SPACE: {
            for(i=1; javaIsspace(z[i]); i++){}
            *tokenType = HC_SPACE;
            return i;
        }
        case CC_EQ: {
            *tokenType = HC_EQ;
            return 1;
        }
        case CC_COMMA: {
            *tokenType = HC_COMMA;
            return 1;
        }
        case CC_QUOTE: {
            int delim = z[0];
            for(i=1; (c=z[i])!=0; i++){
                if( c==delim ){
                    if( z[i+1]==delim ){
                        i++;
                    }else{
                        break;
                    }
                }
            }
            if( c=='\'' ){
                *tokenType = HC_TEXT;
                return i+1;
            }else if( c!=0 ){
                *tokenType = HC_ID;
                return i+1;
            }else{
                *tokenType = HC_ILLEGAL;
                return i;
            }
        }
        case CC_DIGIT: {
            *tokenType = HC_INTEGER;
#ifndef SQLITE_OMIT_HEX_INTEGER
            if( z[0]=='0' && (z[1]=='x' || z[1]=='X') && javaIsxdigit(z[2]) ){
                for(i=3; javaIsxdigit(z[i]); i++){}
                return i;
            }
#endif
            for(i=0; javaIsdigit(z[i]); i++){}
#ifndef SQLITE_OMIT_FLOATING_POINT
            if( z[i]=='.' ){
                i++;
                while( javaIsdigit(z[i]) ){ i++; }
                *tokenType = HC_FLOAT;
            }
            if( (z[i]=='e' || z[i]=='E') &&
               ( javaIsdigit(z[i+1])
                || ((z[i+1]=='+' || z[i+1]=='-') && javaIsdigit(z[i+2]))
                )
               ){
                i += 2;
                while( javaIsdigit(z[i]) ){ i++; }
                *tokenType = HC_FLOAT;
            }
#endif
            while( IdChar(z[i]) ){
                *tokenType = HC_ILLEGAL;
                i++;
            }
            return i;
        }
        case CC_KYWD: {
            for(i=1; aiClass[z[i]]<=CC_KYWD; i++){}
            if( IdChar(z[i]) ){
                /* This token started out using characters that can appear in keywords,
                 ** but z[i] is a character not allowed within keywords, so this must
                 ** be an identifier instead */
                i++;
                break;
            }
            *tokenType = HC_ID;
            return keywordCode((char*)z, i, tokenType);
        }
        case CC_ID: {
            i = 1;
            break;
        }
        case CC_NUL: {
            *tokenType = HC_ILLEGAL;
            return 0;
        }
        case CC_LD:{
             *tokenType = HC_LD;
             return 1;
        }
        case CC_RD:{
            *tokenType = HC_RD;
             return 1;
        }
        case CC_LP:{
            *tokenType = HC_LP;
             return 1;
        }
        case CC_RP:{
            *tokenType = HC_RP;
            return 1;
        }
        case CC_PLUS:{
            *tokenType = HC_PLUS;
            return 1;
        }
        case CC_MINUS:{
            *tokenType = HC_MINUS;
            return 1;
        }
        case CC_MULTI:{
            *tokenType = HC_MULTI;
            return 1;
        }
        case CC_DIVIDE:{
            *tokenType = HC_DIVIDE;
            return 1;
        }
        default: {
            printf("%c",*z);
            *tokenType = HC_ILLEGAL;
            return 1;
        }
    }
    while( IdChar(z[i]) ){ i++; }
    *tokenType = HC_ID;
    return i;
}

/* Check to see if z[0..n-1] is a keyword. If it is, write the
 ** parser symbol code for that keyword into *pType.  Always
 ** return the integer n (the length of the token). */
int keywordCode(const char *z, int n, int *pType){
    int i, j;
    const char *zKW;
    if( n>=2 ){
        i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127;
        for(i=((int)aKWHash[i]); i>=0; i=((int)aKWNext[i])-1){
            if( aKWLen[i]!=n ) continue;
            j = 0;
            zKW = &zKWText[aKWOffset[i]];
            while( j<n && (z[j]&~0x20)==zKW[j] ){ j++; }
            if( j<n ) continue;
            *pType = aKWCode[i];
            break;
        }
    }
    return n;
}

void testKeywordCode(const char *z, int n){
    if( n>=2 ){
       int i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127;
       printf("%s\t%d\n",z,i);
    }
}

void printTokenType(int tokenType){
  
    switch(tokenType){
      case HC_EQ:
        printf("HC_EQ\n");
        break;
      case HC_CLASS:
        printf("HC_CLASS\n");
        break;
      case HC_NEW:
        printf("HC_NEW\n");
        break;
      case HC_INT:
        printf("HC_INT\n");
        break;
      case HC_STRING:
        printf("HC_STRING\n");
        break;
      case HC_PUBLIC:
        printf("HC_PUBLIC\n");
        break;
      case HC_PRIVATE:
        printf("HC_PRIVATE\n");
        break;
      case HC_PROTECTED:
        printf("HC_PROTECTED\n");
        break;
      case HC_RETURN:
        printf("HC_RETURN\n");
        break;
      case HC_EXTENDS:
        printf("HC_EXTENDS\n");
        break;
      case HC_VOID:
        printf("HC_VOID\n");
        break;
      case HC_COMMA:
        printf("HC_COMMA\n");
        break;
      case HC_LP:
        printf("HC_LP\n");
        break;
      case HC_RP:
        printf("HC_RP\n");
        break;
      case HC_TEXT:
        printf("HC_TEXT\n");
        break;
      case HC_INTEGER:
        printf("HC_INTEGER\n");
        break;
      case HC_DOT:
        printf("HC_DOT\n");
        break;
      case HC_SPACE:
        printf("HC_SPACE\n");
        break;
      case HC_ILLEGAL:
        printf("HC_ILLEGAL\n");
        break;
      case HC_SEMI:
        printf("HC_SEMI\n");
        break;
      case HC_LD:
        printf("HC_LD\n");
        break;
      case HC_RD:
        printf("HC_RD\n");
        break;
      case HC_ID:
        printf("HC_ID\n");
        break;
      case HC_FLOAT:
        printf("HC_FLOAT\n");
        break;
      case HC_PLUS:
        printf("HC_PLUS\n");  
        break;
      case HC_MINUS:
        printf("HC_MINUS\n");  
        break;
      case HC_MULTI:
        printf("HC_MULTI\n");
        break;
      case HC_DIVIDE:
        printf("HC_DIVIDE\n");
        break;
      case HC_THIS:
        printf("HC_THIS\n");
        break;
      default:
        printf("invalild\n");
        break;
  
    } 
  
}