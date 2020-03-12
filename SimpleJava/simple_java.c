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
/************ Begin %include sections from the grammar ************************/
#line 6 "simple_java.y"

	#include "simple_java.h"
	#include <assert.h>
	#include <stdlib.h>
    #include "semantic.h"
    #include "simple_java_parser.h"
#line 34 "simple_java.c"
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
#define YYNOCODE 45
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  JavaExpr* yy4;
  JavaExprList * yy16;
  CallTokensList * yy19;
  MetaJavaClassList * yy30;
  JavaFunction * yy55;
  JavaParameterlist * yy62;
  JavaExpr * yy72;
  JavaClassItems * yy77;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL JavaParser *pParse;
#define ParseARG_PDECL ,JavaParser *pParse
#define ParseARG_FETCH JavaParser *pParse = yypParser->pParse
#define ParseARG_STORE yypParser->pParse = pParse
#define YYNSTATE             55
#define YYNRULE              54
#define YY_MAX_SHIFT         54
#define YY_MIN_SHIFTREDUCE   88
#define YY_MAX_SHIFTREDUCE   141
#define YY_MIN_REDUCE        142
#define YY_MAX_REDUCE        195
#define YY_ERROR_ACTION      196
#define YY_ACCEPT_ACTION     197
#define YY_NO_ACTION         198
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
#define YY_ACTTAB_COUNT (117)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    16,  153,  115,  118,   46,   42,   98,  116,   39,   38,
 /*    10 */    13,   13,  197,   20,  117,  119,   35,   25,   34,    2,
 /*    20 */    22,  132,  131,  139,  134,  114,  140,   52,    8,   89,
 /*    30 */    22,   22,   11,   48,   48,   30,   28,   13,   13,   48,
 /*    40 */    17,   17,   17,   34,  122,  129,  132,  131,   19,   45,
 /*    50 */    50,    5,  129,   49,   51,   37,    7,   21,   23,   53,
 /*    60 */    30,   28,   43,    7,  109,   31,    1,   18,  142,   54,
 /*    70 */     9,   15,    3,  137,  135,  133,  123,   24,   27,  130,
 /*    80 */   127,  144,   26,  121,   29,   32,  144,   33,    6,  144,
 /*    90 */   144,  144,   97,   95,  144,    4,   36,  144,   40,  100,
 /*   100 */   101,  144,   44,   12,  102,   47,   41,   96,  144,  144,
 /*   110 */    14,  144,  112,   10,  104,  111,  110,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,   10,    4,   29,   30,    2,   28,   33,    5,    6,
 /*    10 */    12,   13,   24,   25,   40,   41,   42,   19,    2,   21,
 /*    20 */     2,    5,    6,   33,    8,   38,   36,   35,   10,    4,
 /*    30 */    12,   13,    3,   12,   13,   19,   20,   12,   13,   18,
 /*    40 */    15,   16,   17,    2,    2,   30,    5,    6,    2,    7,
 /*    50 */     2,    9,   30,    5,    6,   31,   32,   22,   43,   26,
 /*    60 */    19,   20,   31,   32,   11,   43,   39,   14,    0,    1,
 /*    70 */    34,   37,   27,    2,    2,    8,    2,    7,    2,   10,
 /*    80 */     2,   44,    9,    8,    7,    2,   44,    7,    9,   44,
 /*    90 */    44,   44,    8,    8,   44,   14,   10,   44,    2,   11,
 /*   100 */    11,   44,    2,    9,   11,    2,    7,   10,   44,   44,
 /*   110 */     3,   44,   11,    2,    8,   11,   11,
};
#define YY_SHIFT_USE_DFLT (-10)
#define YY_SHIFT_COUNT (54)
#define YY_SHIFT_MIN   (-9)
#define YY_SHIFT_MAX   (111)
static const signed char yy_shift_ofst[] = {
 /*     0 */   -10,   -2,   16,   25,   41,   -9,   -9,    3,   29,   46,
 /*    10 */    35,  -10,  -10,  -10,  -10,   18,   42,   21,   48,   53,
 /*    20 */    68,   71,   72,   67,   74,   70,   69,   73,   76,   78,
 /*    30 */    77,   75,   79,   83,   80,   81,   84,   86,   88,   89,
 /*    40 */    93,   96,   99,   97,   79,  100,   85,   94,  103,  101,
 /*    50 */   104,  105,  106,  107,  111,
};
#define YY_REDUCE_USE_DFLT (-27)
#define YY_REDUCE_COUNT (14)
#define YY_REDUCE_MIN   (-26)
#define YY_REDUCE_MAX   (45)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -12,  -26,   15,  -10,   22,   24,   31,  -22,  -13,   -8,
 /*    10 */    33,   27,   34,   36,   45,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   144,  196,  196,  196,  196,  157,  157,  196,  196,  196,
 /*    10 */   192,  174,  190,  167,  195,  196,  178,  196,  196,  159,
 /*    20 */   196,  196,  196,  196,  196,  196,  196,  196,  196,  196,
 /*    30 */   196,  196,  182,  196,  180,  196,  196,  196,  147,  146,
 /*    40 */   148,  196,  145,  196,  179,  196,  196,  196,  196,  162,
 /*    50 */   161,  160,  196,  196,  196,
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
  "RD",            "TEXT",          "INTEGER",       "DOT",         
  "SEMI",          "LP",            "RP",            "COMMA",       
  "INT",           "STRING",        "EQ",            "PUBLIC",      
  "PROTECTED",     "PRIVATE",       "VOID",          "THIS",        
  "NEW",           "RETURN",        "EXTENDS",       "error",       
  "javabegin",     "javaclass",     "javasinherit",  "classitems",  
  "expr",          "callexpr",      "callNoSemi",    "callparameterlist",
  "callidist",     "declarevar",    "declarevaritems",  "declareexpr", 
  "function",      "parameterlist",  "functionbody",  "funcbodyitems",
  "assign",        "returnval",     "leftval",       "rightval",    
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "javabegin ::= javaclass",
 /*   1 */ "javaclass ::= javaclass CLASS ID javasinherit LD classitems RD",
 /*   2 */ "javaclass ::=",
 /*   3 */ "expr ::= ID",
 /*   4 */ "expr ::= TEXT",
 /*   5 */ "expr ::= INTEGER",
 /*   6 */ "expr ::= ID DOT ID",
 /*   7 */ "callexpr ::= callNoSemi SEMI",
 /*   8 */ "callNoSemi ::= ID DOT ID LP callparameterlist RP",
 /*   9 */ "callexpr ::= ID LP callparameterlist RP SEMI",
 /*  10 */ "callparameterlist ::= callidist expr",
 /*  11 */ "callparameterlist ::=",
 /*  12 */ "callidist ::= callidist INTEGER COMMA",
 /*  13 */ "callidist ::= callidist TEXT COMMA",
 /*  14 */ "callidist ::= callidist ID DOT ID COMMA",
 /*  15 */ "callidist ::=",
 /*  16 */ "declarevar ::= INT|STRING declarevaritems declareexpr SEMI",
 /*  17 */ "declareexpr ::= ID",
 /*  18 */ "declareexpr ::= ID EQ INTEGER",
 /*  19 */ "declareexpr ::= ID EQ ID",
 /*  20 */ "declareexpr ::= ID EQ TEXT",
 /*  21 */ "declarevaritems ::= declarevaritems ID COMMA",
 /*  22 */ "declarevaritems ::= declarevaritems ID EQ INTEGER COMMA",
 /*  23 */ "declarevaritems ::= declarevaritems ID EQ ID COMMA",
 /*  24 */ "declarevaritems ::= declarevaritems ID EQ TEXT COMMA",
 /*  25 */ "declarevaritems ::=",
 /*  26 */ "function ::= PUBLIC|PROTECTED|PRIVATE INT|STRING|VOID ID LP parameterlist RP functionbody",
 /*  27 */ "functionbody ::= LD funcbodyitems RD",
 /*  28 */ "funcbodyitems ::= funcbodyitems declarevar",
 /*  29 */ "funcbodyitems ::= funcbodyitems assign",
 /*  30 */ "funcbodyitems ::= funcbodyitems callexpr",
 /*  31 */ "funcbodyitems ::= funcbodyitems returnval",
 /*  32 */ "funcbodyitems ::=",
 /*  33 */ "assign ::= leftval EQ rightval SEMI",
 /*  34 */ "leftval ::= ID ID",
 /*  35 */ "leftval ::= THIS DOT ID",
 /*  36 */ "leftval ::= ID",
 /*  37 */ "leftval ::= ID DOT ID",
 /*  38 */ "rightval ::= ID",
 /*  39 */ "rightval ::= THIS DOT ID",
 /*  40 */ "rightval ::= ID DOT ID",
 /*  41 */ "rightval ::= callNoSemi",
 /*  42 */ "rightval ::= NEW ID LP RP",
 /*  43 */ "rightval ::= INTEGER",
 /*  44 */ "rightval ::= TEXT",
 /*  45 */ "returnval ::= RETURN rightval SEMI",
 /*  46 */ "returnval ::= RETURN SEMI",
 /*  47 */ "parameterlist ::= parameterlist INT|STRING|ID ID",
 /*  48 */ "parameterlist ::=",
 /*  49 */ "javasinherit ::= EXTENDS ID",
 /*  50 */ "javasinherit ::=",
 /*  51 */ "classitems ::= classitems declarevar",
 /*  52 */ "classitems ::= classitems function",
 /*  53 */ "classitems ::=",
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
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

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
  { 24, 1 },
  { 25, 7 },
  { 25, 0 },
  { 28, 1 },
  { 28, 1 },
  { 28, 1 },
  { 28, 3 },
  { 29, 2 },
  { 30, 6 },
  { 29, 5 },
  { 31, 2 },
  { 31, 0 },
  { 32, 3 },
  { 32, 3 },
  { 32, 5 },
  { 32, 0 },
  { 33, 4 },
  { 35, 1 },
  { 35, 3 },
  { 35, 3 },
  { 35, 3 },
  { 34, 3 },
  { 34, 5 },
  { 34, 5 },
  { 34, 5 },
  { 34, 0 },
  { 36, 7 },
  { 38, 3 },
  { 39, 2 },
  { 39, 2 },
  { 39, 2 },
  { 39, 2 },
  { 39, 0 },
  { 40, 4 },
  { 42, 2 },
  { 42, 3 },
  { 42, 1 },
  { 42, 3 },
  { 43, 1 },
  { 43, 3 },
  { 43, 3 },
  { 43, 1 },
  { 43, 4 },
  { 43, 1 },
  { 43, 1 },
  { 41, 3 },
  { 41, 2 },
  { 37, 3 },
  { 37, 0 },
  { 26, 2 },
  { 26, 0 },
  { 27, 2 },
  { 27, 2 },
  { 27, 0 },
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
      case 0: /* javabegin ::= javaclass */
#line 59 "simple_java.y"
{
	printf("0000000\n");
}
#line 852 "simple_java.c"
        break;
      case 1: /* javaclass ::= javaclass CLASS ID javasinherit LD classitems RD */
#line 63 "simple_java.y"
{
	printf("rule--->\t1111111\n");
	createClass(pParse,yymsp[-4].minor.yy0,yymsp[-3].minor.yy0,yymsp[-1].minor.yy77);
}
#line 860 "simple_java.c"
        break;
      case 2: /* javaclass ::= */
#line 68 "simple_java.y"
{
    printf("javaclass nil\n");
}
#line 867 "simple_java.c"
        break;
      case 3: /* expr ::= ID */
#line 72 "simple_java.y"
{
	printf("rule--->\t333333333\n");
	yygotominor.yy4 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 875 "simple_java.c"
        break;
      case 4: /* expr ::= TEXT */
#line 76 "simple_java.y"
{
	printf("rule--->\t4444444444\n");
	yygotominor.yy4 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 883 "simple_java.c"
        break;
      case 5: /* expr ::= INTEGER */
#line 80 "simple_java.y"
{
	printf("rule--->\t555555555\n");
	yygotominor.yy4 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 891 "simple_java.c"
        break;
      case 6: /* expr ::= ID DOT ID */
#line 85 "simple_java.y"
{
	printf("rule--->\t6666666666\n");
	yygotominor.yy4 =  instanceGetProperty(yymsp[-2].major,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
}
#line 899 "simple_java.c"
        break;
      case 7: /* callexpr ::= callNoSemi SEMI */
#line 92 "simple_java.y"
{
	printf("rule--->\t777777777777\n");
    yygotominor.yy4 = yymsp[-1].minor.yy4;
}
#line 907 "simple_java.c"
        break;
      case 8: /* callNoSemi ::= ID DOT ID LP callparameterlist RP */
#line 96 "simple_java.y"
{
    yygotominor.yy4 = dressedCallExpr(yymsp[-5].minor.yy0,yymsp[-3].minor.yy0,yymsp[-1].minor.yy19);
}
#line 914 "simple_java.c"
        break;
      case 9: /* callexpr ::= ID LP callparameterlist RP SEMI */
#line 99 "simple_java.y"
{
	printf("rule--->\t888888888\n");
	Token token;
	token.z = 0;
	token.n = 0;
	yygotominor.yy4 = dressedCallExpr(token,yymsp[-4].minor.yy0,yymsp[-2].minor.yy19);
}
#line 925 "simple_java.c"
        break;
      case 10: /* callparameterlist ::= callidist expr */
#line 108 "simple_java.y"
{
	printf("rule--->\t999999999\n");
	yygotominor.yy19 = addTokenToCallParameterList(yymsp[-1].minor.yy19,yymsp[0].minor.yy4);
}
#line 933 "simple_java.c"
        break;
      case 11: /* callparameterlist ::= */
#line 113 "simple_java.y"
{
	printf("rule--->\taaaaaaaaaa\n");
	yygotominor.yy19 = 0;
}
#line 941 "simple_java.c"
        break;
      case 12: /* callidist ::= callidist INTEGER COMMA */
#line 118 "simple_java.y"
{
	printf("rule--->\tbbbbbbbbb\n");
	JavaExpr *expr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
	yygotominor.yy19 = addTokenToCallParameterList(yymsp[-2].minor.yy19,expr);
}
#line 950 "simple_java.c"
        break;
      case 13: /* callidist ::= callidist TEXT COMMA */
#line 124 "simple_java.y"
{
	printf("rule--->\tccccccccccc\n");
	JavaExpr *expr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
	yygotominor.yy19 = addTokenToCallParameterList(yymsp[-2].minor.yy19,expr);
}
#line 959 "simple_java.c"
        break;
      case 14: /* callidist ::= callidist ID DOT ID COMMA */
#line 129 "simple_java.y"
{
	printf("rule--->\tddddddddddd\n");
	JavaExpr *expr = instanceGetProperty(yymsp[-3].major,yymsp[-3].minor.yy0,yymsp[-1].minor.yy0);
	yygotominor.yy19 = addTokenToCallParameterList(yymsp[-4].minor.yy19,expr);
}
#line 968 "simple_java.c"
        break;
      case 15: /* callidist ::= */
#line 134 "simple_java.y"
{
	printf("rule--->\teeeeeeeeeeee\n");
	yygotominor.yy19 = 0;
}
#line 976 "simple_java.c"
        break;
      case 16: /* declarevar ::= INT|STRING declarevaritems declareexpr SEMI */
#line 143 "simple_java.y"
{
	printf("rule--->\tfffffffffff\n");
	yygotominor.yy4 = finishDeclareVars(yymsp[-3].major,yymsp[-2].minor.yy4,yymsp[-1].minor.yy4);
}
#line 984 "simple_java.c"
        break;
      case 17: /* declareexpr ::= ID */
#line 148 "simple_java.y"
{
    yygotominor.yy4 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 991 "simple_java.c"
        break;
      case 18: /* declareexpr ::= ID EQ INTEGER */
      case 19: /* declareexpr ::= ID EQ ID */ yytestcase(yyruleno==19);
      case 20: /* declareexpr ::= ID EQ TEXT */ yytestcase(yyruleno==20);
#line 152 "simple_java.y"
{
    JavaExpr *leftExpr = varTokenToExpr(yymsp[-2].minor.yy0,yymsp[-2].major);
    JavaExpr *rightExpr = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
    yygotominor.yy4 = assginmentExpr(leftExpr,rightExpr);
}
#line 1002 "simple_java.c"
        break;
      case 21: /* declarevaritems ::= declarevaritems ID COMMA */
#line 170 "simple_java.y"
{
	printf("rule--->\tggggggggg\n");
	JavaExpr *expr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
	yygotominor.yy4 = addDeclarevarItem(yymsp[-2].minor.yy4,expr);
}
#line 1011 "simple_java.c"
        break;
      case 22: /* declarevaritems ::= declarevaritems ID EQ INTEGER COMMA */
#line 175 "simple_java.y"
{
	printf("rule--->\thhhhhhhhh\n");
    JavaExpr *leftExpr = varTokenToExpr(yymsp[-3].minor.yy0,yymsp[-3].major);
    JavaExpr *rightExpr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	yygotominor.yy4 = addDeclarevarItem(yymsp[-4].minor.yy4,expr);
}
#line 1022 "simple_java.c"
        break;
      case 23: /* declarevaritems ::= declarevaritems ID EQ ID COMMA */
#line 182 "simple_java.y"
{
	printf("rule--->\tiiiiiiiiiiiii\n");
    JavaExpr *leftExpr = varTokenToExpr(yymsp[-3].minor.yy0,yymsp[-3].major);
    JavaExpr *rightExpr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
	yygotominor.yy4 = addDeclarevarItem(yymsp[-4].minor.yy4,expr);
}
#line 1033 "simple_java.c"
        break;
      case 24: /* declarevaritems ::= declarevaritems ID EQ TEXT COMMA */
#line 189 "simple_java.y"
{
	printf("rule--->\tjjjjjjjjjjjjj\n");
    JavaExpr *leftExpr = varTokenToExpr(yymsp[-3].minor.yy0,yymsp[-3].major);
    JavaExpr *rightExpr = varTokenToExpr(yymsp[-1].minor.yy0,yymsp[-1].major);
    JavaExpr *expr = assginmentExpr(leftExpr,rightExpr);
    yygotominor.yy4 = addDeclarevarItem(yymsp[-4].minor.yy4,expr);
}
#line 1044 "simple_java.c"
        break;
      case 25: /* declarevaritems ::= */
#line 196 "simple_java.y"
{
	printf("rule--->\tkkkkkkkkkk\n");
	yygotominor.yy4 = 0;
}
#line 1052 "simple_java.c"
        break;
      case 26: /* function ::= PUBLIC|PROTECTED|PRIVATE INT|STRING|VOID ID LP parameterlist RP functionbody */
#line 204 "simple_java.y"
{
	printf("rule--->\tlllllllllllll\n");
	yygotominor.yy55 = createFunction(pParse,yymsp[-6].major,yymsp[-5].major,yymsp[-4].minor.yy0,yymsp[-2].minor.yy62,yymsp[0].minor.yy16);
}
#line 1060 "simple_java.c"
        break;
      case 27: /* functionbody ::= LD funcbodyitems RD */
#line 212 "simple_java.y"
{
	printf("rule--->\tmmmmmmmmmmmmm\n");
	yygotominor.yy16 = yymsp[-1].minor.yy16;
}
#line 1068 "simple_java.c"
        break;
      case 28: /* funcbodyitems ::= funcbodyitems declarevar */
#line 217 "simple_java.y"
{
	printf("rule--->\tnnnnnnnnnnnn\n");
	yygotominor.yy16 = addExprToList(yymsp[-1].minor.yy16,yymsp[0].minor.yy4);
}
#line 1076 "simple_java.c"
        break;
      case 29: /* funcbodyitems ::= funcbodyitems assign */
#line 222 "simple_java.y"
{
	printf("rule--->\tooooooooooooooo\n");
	yygotominor.yy16 = addExprToList(yymsp[-1].minor.yy16,yymsp[0].minor.yy72);
}
#line 1084 "simple_java.c"
        break;
      case 30: /* funcbodyitems ::= funcbodyitems callexpr */
#line 226 "simple_java.y"
{
	printf("rule--->\tnnnnnnnnnnnnn\n");
	yygotominor.yy16 = addExprToList(yymsp[-1].minor.yy16,yymsp[0].minor.yy4);
}
#line 1092 "simple_java.c"
        break;
      case 31: /* funcbodyitems ::= funcbodyitems returnval */
#line 230 "simple_java.y"
{
	printf("rule--->\tpppppppppppppp\n");
	yygotominor.yy16 = addExprToList(yymsp[-1].minor.yy16,yymsp[0].minor.yy72);
}
#line 1100 "simple_java.c"
        break;
      case 32: /* funcbodyitems ::= */
#line 236 "simple_java.y"
{
	printf("rule--->\tqqqqqqqqqq\n");
	yygotominor.yy16 = 0;
}
#line 1108 "simple_java.c"
        break;
      case 33: /* assign ::= leftval EQ rightval SEMI */
#line 244 "simple_java.y"
{
	printf("rule--->\trrrrrrrrrrrrr\n");
	yygotominor.yy72 = assginmentExpr(yymsp[-3].minor.yy72,yymsp[-1].minor.yy72);
}
#line 1116 "simple_java.c"
        break;
      case 34: /* leftval ::= ID ID */
#line 249 "simple_java.y"
{
	printf("rule--->\tsssssssssssssss\n");
	yygotominor.yy72 = declareClassVar(yymsp[-1].minor.yy0,yymsp[0].minor.yy0);
}
#line 1124 "simple_java.c"
        break;
      case 35: /* leftval ::= THIS DOT ID */
#line 254 "simple_java.y"
{
	printf("rule--->\tttttttttttttttttt\n");
	yygotominor.yy72 = getClassProperty(yymsp[0].minor.yy0);
}
#line 1132 "simple_java.c"
        break;
      case 36: /* leftval ::= ID */
#line 259 "simple_java.y"
{
	printf("rule--->\tuuuuuuuuuuuuuu\n");
	yygotominor.yy72 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 1140 "simple_java.c"
        break;
      case 37: /* leftval ::= ID DOT ID */
#line 263 "simple_java.y"
{
	printf("rule--->\tvvvvvvvvvvvvvvvvvv\n");
	yygotominor.yy72 =  instanceGetProperty(yymsp[-2].major,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
}
#line 1148 "simple_java.c"
        break;
      case 38: /* rightval ::= ID */
#line 267 "simple_java.y"
{
	printf("rule--->\twwwwwwwwwwwwwww\n");
	yygotominor.yy72 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 1156 "simple_java.c"
        break;
      case 39: /* rightval ::= THIS DOT ID */
#line 272 "simple_java.y"
{
	printf("rule--->\txxxxxxxxxxxxxxxx\n");
	yygotominor.yy72 =  instanceGetProperty(yymsp[-2].major,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
}
#line 1164 "simple_java.c"
        break;
      case 40: /* rightval ::= ID DOT ID */
#line 277 "simple_java.y"
{
	printf("rule--->\tfafafafafa\n");
	yygotominor.yy72 =  instanceGetProperty(yymsp[-2].major,yymsp[-2].minor.yy0,yymsp[0].minor.yy0);
}
#line 1172 "simple_java.c"
        break;
      case 41: /* rightval ::= callNoSemi */
#line 282 "simple_java.y"
{
    yygotominor.yy72 = yymsp[0].minor.yy4;
}
#line 1179 "simple_java.c"
        break;
      case 42: /* rightval ::= NEW ID LP RP */
#line 286 "simple_java.y"
{
	printf("rule--->\tyyyyyyyyyyyyyy\n");
   yygotominor.yy72 = newClsInstance(yymsp[-2].minor.yy0);
}
#line 1187 "simple_java.c"
        break;
      case 43: /* rightval ::= INTEGER */
#line 290 "simple_java.y"
{
	printf("rule--->\tzzzzzzzzzzzzzzzzzzz\n");
	yygotominor.yy72 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 1195 "simple_java.c"
        break;
      case 44: /* rightval ::= TEXT */
#line 294 "simple_java.y"
{
	printf("rule--->\tAAAAAAAAAAAAAAA\n");
	yygotominor.yy72 = varTokenToExpr(yymsp[0].minor.yy0,yymsp[0].major);
}
#line 1203 "simple_java.c"
        break;
      case 45: /* returnval ::= RETURN rightval SEMI */
#line 299 "simple_java.y"
{
	printf("rule--->\tBBBBBBBBBBBBBBBB\n");
	yygotominor.yy72 = returnExpr(yymsp[-1].minor.yy72);
}
#line 1211 "simple_java.c"
        break;
      case 46: /* returnval ::= RETURN SEMI */
#line 303 "simple_java.y"
{
	printf("rule--->\tCCCCCCCCCCCCCCCCCC\n");
	yygotominor.yy72 = 0;
}
#line 1219 "simple_java.c"
        break;
      case 47: /* parameterlist ::= parameterlist INT|STRING|ID ID */
#line 308 "simple_java.y"
{
	printf("rule--->\tDDDDDDDDDDDDDDDDDDDD\n");
	yygotominor.yy62 = addParameter(yymsp[-2].minor.yy62,yymsp[-1].major,yymsp[0].minor.yy0);
}
#line 1227 "simple_java.c"
        break;
      case 48: /* parameterlist ::= */
#line 312 "simple_java.y"
{
	printf("rule--->\tEEEEEEEEEEEEEEEEE\n");
	yygotominor.yy62 = 0;
}
#line 1235 "simple_java.c"
        break;
      case 49: /* javasinherit ::= EXTENDS ID */
#line 317 "simple_java.y"
{
	printf("rule--->\tFFFFFFFFFFFFFFFFF\n");
	yygotominor.yy0 = yymsp[0].minor.yy0;
}
#line 1243 "simple_java.c"
        break;
      case 50: /* javasinherit ::= */
#line 321 "simple_java.y"
{
	printf("rule--->\tGGGGGGGGGGGGGG\n");
    Token token;
	token.z = 0;
	token.n = 0;
	yygotominor.yy0 = token;
}
#line 1254 "simple_java.c"
        break;
      case 51: /* classitems ::= classitems declarevar */
#line 330 "simple_java.y"
{
	printf("rule--->\tHHHHHHHHHHHHHH\n");
  	yygotominor.yy77 = dressedClassBodyWithVars(yymsp[-1].minor.yy77,yymsp[0].minor.yy4);
}
#line 1262 "simple_java.c"
        break;
      case 52: /* classitems ::= classitems function */
#line 335 "simple_java.y"
{
	printf("rule--->\tIIIIIIIIIIII\n");
    if(yymsp[0].minor.yy55 != 0){
        yygotominor.yy77 = dressedClassBodyWithFunc(yymsp[-1].minor.yy77,yymsp[0].minor.yy55);
    }else{
        yygotominor.yy77 = yymsp[-1].minor.yy77;
    }
	
}
#line 1275 "simple_java.c"
        break;
      case 53: /* classitems ::= */
#line 345 "simple_java.y"
{
	printf("rule--->\tJJJJJJJJJJJJJJJ\n");
	yygotominor.yy77 = 0;
}
#line 1283 "simple_java.c"
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
#line 49 "simple_java.y"

	char *errSyntaxMsg = (char *)malloc(TOKEN.n);
	memcpy(errSyntaxMsg,(void*)TOKEN.z,TOKEN.n);
	printf("near %s syntax error！\n",errSyntaxMsg);
	free(errSyntaxMsg);
#line 1355 "simple_java.c"
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
