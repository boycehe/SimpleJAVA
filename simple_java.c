/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "simple_java.h"
/************ Begin %include sections from the grammar ************************/
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 48
#define YYACTIONTYPE unsigned char
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL JavaParser *pParse;
#define ParseARG_FETCH JavaParser *pParse = yypParser->pParse
#define ParseARG_STORE yypParser->pParse = pParse
#define YYNSTATE             60
#define YYNRULE              47
#define YY_MAX_SHIFT         59
#define YY_MIN_SHIFTREDUCE   95
#define YY_MAX_SHIFTREDUCE   141
#define YY_MIN_REDUCE        142
#define YY_MAX_REDUCE        188
#define YY_ERROR_ACTION      189
#define YY_ACCEPT_ACTION     190
#define YY_NO_ACTION         191
/************* End control #defines *******************************************/

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (126)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    39,  133,  131,   57,   59,  129,  111,  130,   33,   19,
 /*    10 */   132,   38,  190,  128,  152,   16,   16,   29,  120,  117,
 /*    20 */    22,   22,   22,   51,  122,   16,   16,   10,  120,    6,
 /*    30 */   104,  125,   14,  123,  122,   32,   50,   28,   28,    8,
 /*    40 */   127,   97,   98,  123,   46,   32,   55,    8,  126,   48,
 /*    50 */    47,   54,   56,  100,  100,  100,  100,   25,  139,   11,
 /*    60 */    20,   30,    9,   53,  140,   23,   58,   27,   21,   28,
 /*    70 */    28,   41,   41,   35,    9,  137,    1,  110,   24,   41,
 /*    80 */    43,    9,   13,    2,   26,  135,  124,  121,   31,    3,
 /*    90 */     7,   34,  103,    4,   36,   15,   37,   18,   40,   42,
 /*   100 */    44,  144,   99,  144,   45,  144,  115,  144,    5,  108,
 /*   110 */    49,   96,  106,   52,  107,  142,   17,  144,   12,  144,
 /*   120 */   144,  144,  114,  144,  113,  112,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    31,   32,   33,    4,    1,   36,   15,   38,   39,   18,
 /*    10 */    41,    2,   27,    4,    6,   16,   17,   40,    2,   40,
 /*    20 */    21,   22,   23,   30,    8,   16,   17,    6,    2,   20,
 /*    30 */    30,   15,    3,   17,    8,   19,    2,   16,   17,    5,
 /*    40 */    44,    7,    8,   17,    2,   19,    2,    5,   44,    7,
 /*    50 */     8,    7,    8,   10,   11,   12,   13,   25,   36,    6,
 /*    60 */    43,   34,   35,   19,   42,    2,   28,   45,   43,   16,
 /*    70 */    17,   16,   17,   34,   35,    2,   46,   14,   37,   24,
 /*    80 */    34,   35,    5,   29,    2,    2,   15,    6,    2,    5,
 /*    90 */    18,    6,   14,    5,    2,    5,    9,    8,    2,    6,
 /*   100 */     2,   47,    2,   47,    9,   47,   15,   47,    5,   15,
 /*   110 */     9,    6,   15,    2,   15,    0,    3,   47,    2,   47,
 /*   120 */    47,   47,   15,   47,   15,   15,
};
#define YY_SHIFT_USE_DFLT (-10)
#define YY_SHIFT_COUNT (59)
#define YY_SHIFT_MIN   (-9)
#define YY_SHIFT_MAX   (116)
static const signed char yy_shift_ofst[] = {
 /*     0 */     3,    9,   -1,    8,    8,    8,   16,   26,   34,   42,
 /*    10 */    29,   29,   32,  -10,  -10,  -10,  -10,  -10,   43,   44,
 /*    20 */    21,   53,   55,   -9,   63,   73,   77,   82,   83,   71,
 /*    30 */    81,   84,   86,   72,   78,   85,   88,   92,   87,   89,
 /*    40 */    90,   96,   91,   93,   94,   98,   95,   97,   99,  100,
 /*    50 */   101,  105,  103,  111,  107,  109,  110,  115,  113,  116,
};
#define YY_REDUCE_USE_DFLT (-32)
#define YY_REDUCE_COUNT (17)
#define YY_REDUCE_MIN   (-31)
#define YY_REDUCE_MAX   (54)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -15,  -31,   22,   27,   39,   46,  -23,  -21,   -7,    0,
 /*    10 */    -4,    4,   38,   17,   30,   25,   41,   54,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   189,  148,  189,  156,  156,  156,  189,  189,  189,  189,
 /*    10 */   189,  189,  185,  183,  181,  183,  163,  188,  149,  189,
 /*    20 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*    30 */   189,  189,  189,  189,  189,  189,  166,  189,  165,  189,
 /*    40 */   189,  189,  189,  189,  146,  189,  189,  145,  144,  189,
 /*    50 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "CLASS",         "ID",            "LD",          
  "RD",            "LP",            "RP",            "TEXT",        
  "INTEGER",       "DOT",           "PLUS",          "MULTI",       
  "DIVIDED",       "MINUS",         "SEMI",          "COMMA",       
  "INT",           "STRING",        "EQ",            "NEW",         
  "RETURN",        "PUBLIC",        "PROTECTED",     "PRIVATE",     
  "VOID",          "EXTENDS",       "error",         "javaclass",   
  "javasinherit",  "classitems",    "expr",          "opcal",       
  "sentence",      "callexpr",      "callparameterlist",  "callidist",   
  "declarevar",    "declarevaritems",  "assign",        "leftval",     
  "rightval",      "returnval",     "function",      "parameterlist",
  "functionbody",  "returntype",    "funcbodyitems",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "javaclass ::= CLASS ID javasinherit LD classitems RD",
 /*   1 */ "expr ::= LP expr RP",
 /*   2 */ "expr ::= TEXT",
 /*   3 */ "expr ::= INTEGER",
 /*   4 */ "expr ::= ID DOT ID",
 /*   5 */ "opcal ::= opcal INTEGER PLUS|MULTI|DIVIDED|MINUS",
 /*   6 */ "opcal ::=",
 /*   7 */ "sentence ::= opcal INTEGER",
 /*   8 */ "callexpr ::= ID DOT ID LP callparameterlist RP SEMI",
 /*   9 */ "callparameterlist ::= callidist expr",
 /*  10 */ "callparameterlist ::=",
 /*  11 */ "callidist ::= callidist INTEGER COMMA",
 /*  12 */ "callidist ::= callidist TEXT COMMA",
 /*  13 */ "callidist ::= callidist ID DOT ID COMMA",
 /*  14 */ "callidist ::=",
 /*  15 */ "declarevar ::= INT|STRING declarevaritems SEMI",
 /*  16 */ "declarevaritems ::= declarevaritems ID COMMA",
 /*  17 */ "declarevaritems ::= declarevaritems ID EQ INTEGER COMMA",
 /*  18 */ "declarevaritems ::= declarevaritems ID EQ ID COMMA",
 /*  19 */ "declarevaritems ::= declarevaritems ID EQ TEXT COMMA",
 /*  20 */ "declarevaritems ::= declarevaritems ID EQ NEW ID LP callparameterlist RP COMMA",
 /*  21 */ "declarevaritems ::=",
 /*  22 */ "assign ::= leftval EQ rightval",
 /*  23 */ "leftval ::= ID",
 /*  24 */ "leftval ::= ID DOT ID",
 /*  25 */ "rightval ::= ID",
 /*  26 */ "rightval ::= NEW ID LP callparameterlist RP",
 /*  27 */ "rightval ::= INTEGER",
 /*  28 */ "rightval ::= STRING",
 /*  29 */ "returnval ::= RETURN rightval COMMA",
 /*  30 */ "returnval ::= RETURN COMMA",
 /*  31 */ "function ::= PUBLIC|PROTECTED|PRIVATE INT|STRING|VOID ID LP parameterlist RP functionbody",
 /*  32 */ "function ::= returntype ID LP parameterlist RP functionbody",
 /*  33 */ "functionbody ::= LD funcbodyitems RD",
 /*  34 */ "funcbodyitems ::= funcbodyitems declarevar",
 /*  35 */ "funcbodyitems ::= funcbodyitems assign",
 /*  36 */ "funcbodyitems ::= funcbodyitems callexpr",
 /*  37 */ "funcbodyitems ::= funcbodyitems returnval",
 /*  38 */ "funcbodyitems ::= funcbodyitems sentence",
 /*  39 */ "funcbodyitems ::=",
 /*  40 */ "parameterlist ::= parameterlist INT|STRING ID",
 /*  41 */ "parameterlist ::=",
 /*  42 */ "javasinherit ::= EXTENDS ID",
 /*  43 */ "javasinherit ::=",
 /*  44 */ "classitems ::= classitems declarevar",
 /*  45 */ "classitems ::= classitems function",
 /*  46 */ "classitems ::=",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 27, 6 },
  { 30, 3 },
  { 30, 1 },
  { 30, 1 },
  { 30, 3 },
  { 31, 3 },
  { 31, 0 },
  { 32, 2 },
  { 33, 7 },
  { 34, 2 },
  { 34, 0 },
  { 35, 3 },
  { 35, 3 },
  { 35, 5 },
  { 35, 0 },
  { 36, 3 },
  { 37, 3 },
  { 37, 5 },
  { 37, 5 },
  { 37, 5 },
  { 37, 9 },
  { 37, 0 },
  { 38, 3 },
  { 39, 1 },
  { 39, 3 },
  { 40, 1 },
  { 40, 5 },
  { 40, 1 },
  { 40, 1 },
  { 41, 3 },
  { 41, 2 },
  { 42, 7 },
  { 42, 6 },
  { 44, 3 },
  { 46, 2 },
  { 46, 2 },
  { 46, 2 },
  { 46, 2 },
  { 46, 2 },
  { 46, 0 },
  { 43, 3 },
  { 43, 0 },
  { 28, 2 },
  { 28, 0 },
  { 29, 2 },
  { 29, 2 },
  { 29, 0 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */
  yygotominor = yyzerominor;

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* javaclass ::= CLASS ID javasinherit LD classitems RD */
#line 42 "simple_java.y"
{
	//createClass(pParse,yymsp[-4].minor.yy0,yymsp[-3].minor.yy0,yymsp[-1].minor.yy0);
	printf("275\n");
}
#line 827 "simple_java.c"
        break;
      case 1: /* expr ::= LP expr RP */
#line 48 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[-1].minor.yy0;
	printf("52\n");
}
#line 835 "simple_java.c"
        break;
      case 2: /* expr ::= TEXT */
#line 52 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[0].minor.yy0;
	printf("53\n");
}
#line 843 "simple_java.c"
        break;
      case 3: /* expr ::= INTEGER */
#line 56 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[0].minor.yy0;
	printf("54\n");
}
#line 851 "simple_java.c"
        break;
      case 4: /* expr ::= ID DOT ID */
#line 61 "simple_java.y"
{
	printf("55\n");
}
#line 858 "simple_java.c"
        break;
      case 5: /* opcal ::= opcal INTEGER PLUS|MULTI|DIVIDED|MINUS */
#line 65 "simple_java.y"
{
	printf("91\n");
}
#line 865 "simple_java.c"
        break;
      case 6: /* opcal ::= */
#line 68 "simple_java.y"
{
	printf("95\n");
}
#line 872 "simple_java.c"
        break;
      case 7: /* sentence ::= opcal INTEGER */
#line 72 "simple_java.y"
{
	printf("107\n");
}
#line 879 "simple_java.c"
        break;
      case 8: /* callexpr ::= ID DOT ID LP callparameterlist RP SEMI */
#line 78 "simple_java.y"
{
	printf("116\n");
}
#line 886 "simple_java.c"
        break;
      case 9: /* callparameterlist ::= callidist expr */
#line 81 "simple_java.y"
{
	printf("119\n");
}
#line 893 "simple_java.c"
        break;
      case 10: /* callparameterlist ::= */
#line 84 "simple_java.y"
{
	printf("123\n");
}
#line 900 "simple_java.c"
        break;
      case 11: /* callidist ::= callidist INTEGER COMMA */
#line 88 "simple_java.y"
{
	printf("127\n");
}
#line 907 "simple_java.c"
        break;
      case 12: /* callidist ::= callidist TEXT COMMA */
#line 91 "simple_java.y"
{
	printf("130\n");
}
#line 914 "simple_java.c"
        break;
      case 13: /* callidist ::= callidist ID DOT ID COMMA */
#line 94 "simple_java.y"
{
	printf("133\n");
}
#line 921 "simple_java.c"
        break;
      case 14: /* callidist ::= */
#line 97 "simple_java.y"
{
	//yygotominor.yy0 = 0;
	printf("137\n");
}
#line 929 "simple_java.c"
        break;
      case 15: /* declarevar ::= INT|STRING declarevaritems SEMI */
#line 103 "simple_java.y"
{
	printf("142\n");
}
#line 936 "simple_java.c"
        break;
      case 16: /* declarevaritems ::= declarevaritems ID COMMA */
#line 106 "simple_java.y"
{
	printf("149\n");
}
#line 943 "simple_java.c"
        break;
      case 17: /* declarevaritems ::= declarevaritems ID EQ INTEGER COMMA */
#line 109 "simple_java.y"
{
	printf("152\n");
}
#line 950 "simple_java.c"
        break;
      case 18: /* declarevaritems ::= declarevaritems ID EQ ID COMMA */
      case 19: /* declarevaritems ::= declarevaritems ID EQ TEXT COMMA */ yytestcase(yyruleno==19);
#line 112 "simple_java.y"
{
	printf("155\n");
}
#line 958 "simple_java.c"
        break;
      case 20: /* declarevaritems ::= declarevaritems ID EQ NEW ID LP callparameterlist RP COMMA */
#line 118 "simple_java.y"
{
	printf("158\n");
}
#line 965 "simple_java.c"
        break;
      case 21: /* declarevaritems ::= */
#line 122 "simple_java.y"
{
	//yygotominor.yy0 = 0;
	printf("163\n");
}
#line 973 "simple_java.c"
        break;
      case 22: /* assign ::= leftval EQ rightval */
#line 128 "simple_java.y"
{
	//assignExpr(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	printf("169\n");
}
#line 981 "simple_java.c"
        break;
      case 23: /* leftval ::= ID */
#line 133 "simple_java.y"
{
	//A = yymsp[0].minor.yy0;
	printf("174\n");
}
#line 989 "simple_java.c"
        break;
      case 24: /* leftval ::= ID DOT ID */
#line 137 "simple_java.y"
{
	//yygotominor.yy0 =  getProperty(yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	printf("178\n");
}
#line 997 "simple_java.c"
        break;
      case 25: /* rightval ::= ID */
#line 141 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[0].minor.yy0;
	printf("182\n");
}
#line 1005 "simple_java.c"
        break;
      case 26: /* rightval ::= NEW ID LP callparameterlist RP */
#line 145 "simple_java.y"
{
   //A = newInit(yymsp[-3].minor.yy0,yymsp[-1].minor.yy0);
	printf("186\n");
}
#line 1013 "simple_java.c"
        break;
      case 27: /* rightval ::= INTEGER */
#line 149 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[0].minor.yy0;
	printf("190\n");
}
#line 1021 "simple_java.c"
        break;
      case 28: /* rightval ::= STRING */
#line 153 "simple_java.y"
{
	printf("193\n");
}
#line 1028 "simple_java.c"
        break;
      case 29: /* returnval ::= RETURN rightval COMMA */
#line 157 "simple_java.y"
{
	printf("199\n");
}
#line 1035 "simple_java.c"
        break;
      case 30: /* returnval ::= RETURN COMMA */
#line 160 "simple_java.y"
{
	printf("206\n");
}
#line 1042 "simple_java.c"
        break;
      case 31: /* function ::= PUBLIC|PROTECTED|PRIVATE INT|STRING|VOID ID LP parameterlist RP functionbody */
#line 165 "simple_java.y"
{
	//yygotominor.yy0 = addFunctionToClass(pParse,yymsp[-6].minor.yy0,yymsp[-5].minor.yy0,yymsp[-4].minor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	printf("213\n");
}
#line 1050 "simple_java.c"
        break;
      case 32: /* function ::= returntype ID LP parameterlist RP functionbody */
#line 170 "simple_java.y"
{
	//yygotominor.yy0 = addFunctionToClass(pParse,B,yymsp[-5].minor.yy0,yymsp[-4].minor.yy0,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
	printf("213\n");
}
#line 1058 "simple_java.c"
        break;
      case 33: /* functionbody ::= LD funcbodyitems RD */
#line 175 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[-1].minor.yy0;
	printf("228\n");
}
#line 1066 "simple_java.c"
        break;
      case 34: /* funcbodyitems ::= funcbodyitems declarevar */
#line 180 "simple_java.y"
{
	printf("232\n");
}
#line 1073 "simple_java.c"
        break;
      case 35: /* funcbodyitems ::= funcbodyitems assign */
#line 184 "simple_java.y"
{
	printf("236\n");
}
#line 1080 "simple_java.c"
        break;
      case 36: /* funcbodyitems ::= funcbodyitems callexpr */
#line 187 "simple_java.y"
{
	printf("239\n");
}
#line 1087 "simple_java.c"
        break;
      case 37: /* funcbodyitems ::= funcbodyitems returnval */
#line 190 "simple_java.y"
{
	printf("240\n");
}
#line 1094 "simple_java.c"
        break;
      case 38: /* funcbodyitems ::= funcbodyitems sentence */
#line 194 "simple_java.y"
{
	printf("250\n");
}
#line 1101 "simple_java.c"
        break;
      case 39: /* funcbodyitems ::= */
#line 198 "simple_java.y"
{
	//yygotominor.yy0 = 0;
	printf("243\n");
}
#line 1109 "simple_java.c"
        break;
      case 40: /* parameterlist ::= parameterlist INT|STRING ID */
#line 203 "simple_java.y"
{
	printf("265\n");
}
#line 1116 "simple_java.c"
        break;
      case 41: /* parameterlist ::= */
#line 206 "simple_java.y"
{
	//yygotominor.yy0 = 0.
	printf("269\n");
}
#line 1124 "simple_java.c"
        break;
      case 42: /* javasinherit ::= EXTENDS ID */
#line 211 "simple_java.y"
{
	//yygotominor.yy0 = yymsp[0].minor.yy0;
	printf("280\n");
}
#line 1132 "simple_java.c"
        break;
      case 43: /* javasinherit ::= */
#line 215 "simple_java.y"
{
    //yygotominor.yy0 = 0;
	printf("284\n");
}
#line 1140 "simple_java.c"
        break;
      case 44: /* classitems ::= classitems declarevar */
#line 220 "simple_java.y"
{
    //	yygotominor.yy0 = dressedClassBodyWithVars(yymsp[-1].minor.yy0,yymsp[0].minor.yy0);
	printf("289\n");
}
#line 1148 "simple_java.c"
        break;
      case 45: /* classitems ::= classitems function */
#line 225 "simple_java.y"
{
//	yygotominor.yy0 = dressedClassBodyWithFunc(yymsp[-1].minor.yy0,yymsp[0].minor.yy0);
	printf("294\n");
}
#line 1156 "simple_java.c"
        break;
      case 46: /* classitems ::= */
#line 230 "simple_java.y"
{
//	yygotominor.yy0 = 0.
	printf("299\n");
}
#line 1164 "simple_java.c"
        break;
      default:
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    /* If the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
      yyTraceShift(yypParser, yyact);
    }else{
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
  yyminorunion.yy0 = yyminor;
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
