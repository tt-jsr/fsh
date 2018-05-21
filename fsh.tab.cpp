/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "fsh.y" /* yacc.c:339  */

#include <cstdio>
#include <iostream>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"
#include "ast.h"
#include "shell.h"

using namespace std;

// stuff from flex that bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern uint64_t lineno;
extern uint64_t column;
extern bool commandMode;
extern bool interactive;
void AstExecute(fsh::Ast *pAst);
 
extern void yyerror(const char *s);

typedef fsh::Ast ast_t;
typedef fsh::ASTBinaryOperator bop_t;
typedef fsh::ASTExpressionList el_t;

#line 97 "fsh.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "fsh.tab.h".  */
#ifndef YY_YY_FSH_TAB_H_INCLUDED
# define YY_YY_FSH_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    INTEGER = 259,
    FLOAT = 260,
    STRING_LITERAL = 261,
    NONE = 262,
    TRUE = 263,
    FALSE = 264,
    IF = 265,
    WHILE = 266,
    THEN = 267,
    ELSE = 268,
    TRY = 269,
    CATCH = 270,
    FOR = 271,
    IN = 272,
    KW_RETURN = 273,
    GLOBAL = 274,
    DOUBLE_BRACKET_OPEN = 275,
    DOUBLE_BRACKET_CLOSE = 276,
    SYSTEM = 277,
    CMD_WORD = 278,
    CMD_BAR = 279,
    CMD_LT = 280,
    CMD_GT = 281,
    CMD_AMP = 282,
    CMD_GTGT = 283,
    CMD_SEMI = 284,
    CMD_NL = 285,
    AND = 286,
    OR = 287,
    GT = 288,
    LT = 289,
    GTE = 290,
    LTE = 291,
    EQ = 292,
    NEQ = 293,
    RIGHT_ARROW = 294,
    DOT = 295,
    NEG = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef void * YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_FSH_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 190 "fsh.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   858

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  153

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,     2,
      49,    50,    44,    42,    32,    43,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    31,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    34,    35,    36,    37,
      38,    39,    40,    41,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    70,    70,    71,    75,    79,    80,    81,    82,    83,
      84,    85,    86,    92,    93,    94,    95,    96,   101,   106,
     111,   112,   116,   122,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   146,   156,   163,   170,
     177,   184,   191,   198,   205,   215,   222,   229,   236,   243,
     252,   269,   285,   301,   320,   329,   339,   349,   358,   387,
     395,   402,   410,   416,   425,   428,   450,   455,   465,   474,
     479,   489,   494,   504,   505,   506,   511,   515,   525,   529,
     530
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INTEGER", "FLOAT",
  "STRING_LITERAL", "NONE", "TRUE", "FALSE", "IF", "WHILE", "THEN", "ELSE",
  "TRY", "CATCH", "FOR", "IN", "KW_RETURN", "GLOBAL",
  "DOUBLE_BRACKET_OPEN", "DOUBLE_BRACKET_CLOSE", "SYSTEM", "CMD_WORD",
  "CMD_BAR", "CMD_LT", "CMD_GT", "CMD_AMP", "CMD_GTGT", "CMD_SEMI",
  "CMD_NL", "';'", "','", "'='", "AND", "OR", "GT", "LT", "GTE", "LTE",
  "EQ", "NEQ", "'+'", "'-'", "'*'", "'/'", "RIGHT_ARROW", "DOT", "NEG",
  "'('", "')'", "':'", "'['", "']'", "'&'", "$accept", "input", "toplev",
  "primary_expression", "assignment_expression", "expression",
  "dot_expression", "relational_expression", "math_expression",
  "subscript_expression", "exception_expression", "selection_expression",
  "for_expression", "iteration_expression", "function_definition",
  "function_call", "identifier_list", "expression_list",
  "attribute_expression", "attribute_list", "call_expression_list",
  "expression_statement", "statement", "statement_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    59,    44,    61,   286,   287,   288,   289,   290,   291,
     292,   293,    43,    45,    42,    47,   294,   295,   296,    40,
      41,    58,    91,    93,    38
};
# endif

#define YYPACT_NINF -50

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-50)))

#define YYTABLE_NINF -68

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -50,   136,   -50,   -29,   -50,   -50,   -50,   -50,   -50,   -50,
     -47,   -16,   -11,   -10,   686,    43,   -50,   -50,   -50,   -50,
     -50,   -50,   -50,   -50,   -50,   -50,   270,   270,    -9,   -50,
     -17,   -50,   754,    39,   -50,   -50,   -50,   -50,   -50,   -50,
     -50,   -50,   -50,   -50,   -50,   270,   270,   270,   604,    64,
     -50,   769,    44,   -50,   811,    34,   656,   166,   716,   -50,
     270,   270,   270,   270,   270,   270,   270,   270,   270,   270,
     270,   270,    73,   270,   811,    -2,    27,   -50,   188,    61,
     -50,   -50,   270,   -50,    32,   -20,   240,   270,   707,    40,
     -50,   783,    47,   -50,   -26,   -42,   289,   289,   327,   327,
     327,   327,    55,    55,    -7,    -7,    33,    33,   -50,   811,
     604,   604,   604,   -50,   270,   811,    78,   604,   -50,   -19,
     -50,   218,   270,   270,    84,   -50,    84,   -50,   292,   344,
     396,    77,   -50,   448,   -50,   -50,   739,   811,   797,    42,
     -50,   -24,   604,   -50,   -50,   604,   -50,   -50,   -50,   500,
     552,   -50,   -50
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    13,    14,    15,    16,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    20,     5,     6,     7,
       8,    10,     9,    11,    12,    73,     0,     0,     0,     3,
      24,    25,     0,    26,    28,    27,    35,    33,    31,    34,
      32,    30,    29,    78,     4,     0,     0,     0,     0,     0,
      76,     0,     0,    49,    66,     0,     0,     0,     0,    74,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    22,     0,     0,    79,     0,     0,
      77,    75,     0,    21,    13,     0,     0,     0,     0,    13,
      63,    71,     0,    69,     0,     0,    43,    44,    39,    37,
      40,    38,    41,    42,    45,    46,    47,    48,    36,    23,
       0,     0,     0,    80,     0,    67,     0,     0,    59,     0,
      52,     0,     0,     0,     0,    62,     0,    60,     0,     0,
       0,     0,    65,     0,    53,    51,     0,    68,    72,     0,
      70,     0,     0,    57,    54,     0,    58,    50,    61,     0,
       0,    55,    56
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -50,   -50,   -50,   -50,   -50,   -13,   -50,   -50,   -50,   -50,
     -50,   -50,   -50,   -50,   -50,   -50,   -50,    24,   -34,   -35,
     -50,   -50,    -1,   -49
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    85,    55,    93,    94,
      95,    43,    77,    78
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      44,    51,   134,    57,    45,    46,   124,    86,   124,   126,
     110,   127,   116,    53,    54,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,   125,    72,   148,
      82,   117,    74,    54,    54,    58,    47,    70,    71,   111,
      72,    48,    49,    56,    88,    91,    52,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    82,
     109,   128,   129,   130,   -64,    45,    82,    79,   133,   115,
      75,    76,    73,    45,   119,    81,   108,   113,   114,   123,
      72,   132,    92,   -64,    83,   113,   122,   139,   122,   145,
     140,   141,     0,   149,     0,     0,   150,    68,    69,    70,
      71,   131,    72,     0,     0,     0,     0,     0,   136,   137,
     138,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,     0,    72,     0,     0,   113,   113,   113,
       0,     0,   113,     0,     0,     0,     2,     0,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,   113,   113,
      12,     0,    13,     0,    14,    15,     0,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,    26,
      12,     0,    13,     0,     0,    27,     0,     0,    16,     0,
      28,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,     0,    12,   112,    13,     0,    14,    15,     0,    26,
      16,     0,     0,     0,     0,    27,     0,    87,     0,    25,
      28,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,    26,    12,     0,    13,     0,     0,    27,     0,   135,
      16,     0,    28,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,    12,     0,    13,     0,    14,    15,
       0,    26,    16,     0,     0,     0,     0,    27,     0,     0,
       0,    25,    28,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,    26,    12,     0,    13,     0,     0,    27,
       0,     0,    16,   118,    28,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,   142,    12,     0,    13,     0,
      14,    15,     0,    26,    16,     0,     0,     0,     0,    27,
       0,     0,     0,    25,    28,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    26,    72,     0,     0,     0,
       0,    27,     0,     0,     0,     0,    28,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     0,     0,    12,     0,
      13,     0,    14,    15,     0,     0,    16,    66,    67,    68,
      69,    70,    71,     0,    72,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    27,     0,     0,     0,   143,    28,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,     0,
      12,     0,    13,     0,    14,    15,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,    27,     0,     0,     0,   144,
      28,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,     0,    12,     0,    13,     0,    14,    15,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,    27,     0,     0,
       0,   146,    28,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,    12,     0,    13,     0,    14,    15,
       0,     0,    16,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    26,     0,     0,     0,     0,     0,    27,
       0,     0,     0,   151,    28,     3,     4,     5,     6,     7,
       8,     9,    10,    11,     0,     0,    12,     0,    13,     0,
      14,    15,     0,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,    27,     0,     0,     0,   152,    28,     3,     4,     5,
       6,     7,     8,     9,    10,    11,     0,     0,    12,     0,
      13,     0,    14,    15,     0,     0,    16,     0,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,    28,    84,
       4,     5,     6,     7,     8,     9,    10,    11,     0,     0,
      12,     0,    13,     0,    14,    15,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,    25,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,    26,
      12,     0,    13,     0,     0,    27,     0,     0,    16,     0,
      28,     0,     0,     0,     0,     0,     0,    50,     0,    89,
       4,     5,     6,     7,     8,     9,    10,    11,   120,    26,
      12,     0,    13,     0,     0,    27,     0,     0,    16,     0,
      28,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,     0,    72,     0,     0,     0,   121,    26,
     147,     0,     0,     0,     0,    27,     0,     0,     0,    90,
      28,     0,     0,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    59,    72,     0,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      80,    72,     0,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,   -66,    72,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,   -67,
      72,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,     0,    72,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,    72
};

static const yytype_int16 yycheck[] =
{
       1,    14,    21,    20,    33,    52,    32,    56,    32,    51,
      12,    53,    32,    26,    27,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    53,    47,    53,
      32,    51,    45,    46,    47,    52,    52,    44,    45,    12,
      47,    52,    52,    52,    57,    58,     3,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    32,
      73,   110,   111,   112,    32,    33,    32,     3,   117,    82,
      46,    47,    33,    33,    87,    31,     3,    78,    17,    32,
      47,     3,    58,    51,    50,    86,    46,     3,    46,    12,
     124,   126,    -1,   142,    -1,    -1,   145,    42,    43,    44,
      45,   114,    47,    -1,    -1,    -1,    -1,    -1,   121,   122,
     123,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    -1,    -1,   128,   129,   130,
      -1,    -1,   133,    -1,    -1,    -1,     0,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,   149,   150,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    43,
      14,    -1,    16,    -1,    -1,    49,    -1,    -1,    22,    -1,
      54,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    15,    16,    -1,    18,    19,    -1,    43,
      22,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    31,
      54,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    43,    14,    -1,    16,    -1,    -1,    49,    -1,    21,
      22,    -1,    54,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    16,    -1,    18,    19,
      -1,    43,    22,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    31,    54,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    43,    14,    -1,    16,    -1,    -1,    49,
      -1,    -1,    22,    53,    54,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    14,    -1,    16,    -1,
      18,    19,    -1,    43,    22,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    31,    54,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    43,    47,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    54,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    40,    41,    42,
      43,    44,    45,    -1,    47,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    53,    54,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    53,
      54,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    53,    54,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    -1,    -1,    14,    -1,    16,    -1,    18,    19,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    53,    54,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    -1,    14,    -1,    16,    -1,
      18,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    53,    54,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    -1,    -1,    14,    -1,
      16,    -1,    18,    19,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    54,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    -1,
      14,    -1,    16,    -1,    18,    19,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    43,
      14,    -1,    16,    -1,    -1,    49,    -1,    -1,    22,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    21,    43,
      14,    -1,    16,    -1,    -1,    49,    -1,    -1,    22,    -1,
      54,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    -1,    -1,    -1,    51,    43,
      21,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    53,
      54,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    31,    47,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      31,    47,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    32,    47,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    32,
      47,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    14,    16,    18,    19,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    43,    49,    54,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    76,    77,    33,    52,    52,    52,    52,
      31,    60,     3,    60,    60,    72,    52,    20,    52,    31,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    47,    33,    60,    72,    72,    77,    78,     3,
      31,    31,    32,    50,     3,    71,    78,    51,    60,     3,
      53,    60,    72,    73,    74,    75,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,     3,    60,
      12,    12,    15,    77,    17,    60,    32,    51,    53,    60,
      21,    51,    46,    32,    32,    53,    51,    53,    78,    78,
      78,    60,     3,    78,    21,    21,    60,    60,    60,     3,
      73,    74,    13,    53,    53,    12,    53,    21,    53,    78,
      78,    53,    53
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    56,    56,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    61,    62,    62,    62,
      62,    62,    62,    62,    62,    63,    63,    63,    63,    63,
      64,    64,    64,    64,    65,    66,    67,    68,    69,    69,
      70,    70,    70,    70,    71,    71,    72,    72,    73,    74,
      74,    75,    75,    76,    76,    76,    76,    76,    77,    78,
      78
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       6,     5,     4,     5,     6,     8,     8,     6,     6,     4,
       4,     6,     4,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     2,     3,     2,     3,     1,     1,
       2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 75 "fsh.y" /* yacc.c:1646  */
    {
        ast_t *pAst = (ast_t *)(yyvsp[0]);
        AstExecute(pAst);
    }
#line 1523 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 79 "fsh.y" /* yacc.c:1646  */
    {fsh::PushWord((char *)(yyvsp[0]));}
#line 1529 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 80 "fsh.y" /* yacc.c:1646  */
    {fsh::PushBar();}
#line 1535 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 81 "fsh.y" /* yacc.c:1646  */
    {fsh::PushLT();}
#line 1541 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 82 "fsh.y" /* yacc.c:1646  */
    {fsh::PushGT();}
#line 1547 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 83 "fsh.y" /* yacc.c:1646  */
    {fsh::PushGTGT();}
#line 1553 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 84 "fsh.y" /* yacc.c:1646  */
    {fsh::PushAmp();}
#line 1559 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 85 "fsh.y" /* yacc.c:1646  */
    {fsh::PushSemi();}
#line 1565 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 86 "fsh.y" /* yacc.c:1646  */
    {fsh::PushNL();
            std::cout << "\r> ";
            }
#line 1573 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 92 "fsh.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]);}
#line 1579 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 93 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1585 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 94 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1591 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 95 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1597 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 96 "fsh.y" /* yacc.c:1646  */
    {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_NONE;
                                (yyval) = bc;
                            }
#line 1607 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 101 "fsh.y" /* yacc.c:1646  */
    {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_TRUE;
                                (yyval) = bc;
                            }
#line 1617 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 106 "fsh.y" /* yacc.c:1646  */
    {
                                fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                                bc->ctype = fsh::ASTConstant::CTYPE_FALSE;
                                (yyval) = bc;
                            }
#line 1627 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 111 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1633 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 112 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[-1]);}
#line 1639 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 116 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTAssignment *ass = new fsh::ASTAssignment(lineno);
        ass->rhs.reset((ast_t *)(yyvsp[0]));
        ass->lhs.reset((ast_t *)(yyvsp[-2]));
        (yyval) = ass;
    }
#line 1650 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 122 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTAssignment *ass = new fsh::ASTAssignment(lineno);
        ass->rhs.reset((ast_t *)(yyvsp[0]));
        ass->lhs.reset((ast_t *)(yyvsp[-2]));
        (yyval) = ass;
    }
#line 1661 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 131 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1667 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 132 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1673 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 133 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1679 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 134 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1685 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 135 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1691 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 136 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1697 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 137 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1703 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 138 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1709 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 139 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1715 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 140 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1721 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 141 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1727 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 142 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 1733 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 146 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_DOT;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1745 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 156 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_LT;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1757 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 163 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_LTE;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1769 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 170 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_GT;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1781 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 177 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_GTE;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1793 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 184 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_EQ;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1805 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 191 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_RELATIONAL_NEQ;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1817 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 198 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_LOGICAL_AND;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1829 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 205 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_LOGICAL_OR;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1841 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 215 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_ADD;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1853 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 222 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_SUBTRACT;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1865 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 229 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_MULTIPLY;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1877 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 236 "fsh.y" /* yacc.c:1646  */
    {
        bop_t *bop = new bop_t(lineno);
        bop->op = fsh::BC_BINARY_DIVIDE;
        bop->lhs.reset((ast_t *)(yyvsp[-2]));
        bop->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = bop;
    }
#line 1889 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 243 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTUnaryOperator *uo= new fsh::ASTUnaryOperator(lineno);
        uo->op = fsh::BC_UNARY_NEGATE;
        uo->operand.reset((ast_t *)(yyvsp[0]));
        (yyval) = uo;
    }
#line 1900 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 252 "fsh.y" /* yacc.c:1646  */
    {
        // Set function name
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset(fsh::ASTMakeIdentifierConstant(lineno, "Subscript"));

        fsh::ASTExpressionList *pList = new fsh::ASTExpressionList(lineno);
        pList->expressions.emplace_back((ast_t *)(yyvsp[-5]));
        pList->expressions.emplace_back((ast_t *)(yyvsp[-3]));

        pList->expressions.emplace_back(fsh::ASTMakeBooleanConstant(lineno, true));

        pList->expressions.emplace_back((ast_t *)(yyvsp[-1]));

        pCall->arguments.reset(pList);

        (yyval) = pCall;
    }
#line 1922 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 269 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset(fsh::ASTMakeIdentifierConstant(lineno, "Subscript"));

        fsh::ASTExpressionList *pList = new fsh::ASTExpressionList(lineno);
        pList->expressions.emplace_back((ast_t *)(yyvsp[-4]));
        pList->expressions.emplace_back((ast_t *)(yyvsp[-2]));

        pList->expressions.emplace_back(fsh::ASTMakeBooleanConstant(lineno, true));

        pList->expressions.emplace_back(fsh::ASTMakeNoneConstant(lineno));

        pCall->arguments.reset(pList);

        (yyval) = pCall;
    }
#line 1943 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 285 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset(fsh::ASTMakeIdentifierConstant(lineno, "Subscript"));

        fsh::ASTExpressionList *pList = new fsh::ASTExpressionList(lineno);
        pList->expressions.emplace_back((ast_t *)(yyvsp[-3]));
        pList->expressions.emplace_back((ast_t *)(yyvsp[-1]));

        pList->expressions.emplace_back(fsh::ASTMakeBooleanConstant(lineno, false));

        pList->expressions.emplace_back(fsh::ASTMakeNoneConstant(lineno));

        pCall->arguments.reset(pList);

        (yyval) = pCall;
    }
#line 1964 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 301 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset(fsh::ASTMakeIdentifierConstant(lineno, "Subscript"));

        fsh::ASTExpressionList *pList = new fsh::ASTExpressionList(lineno);
        pList->expressions.emplace_back((ast_t *)(yyvsp[-4]));
        pList->expressions.emplace_back(fsh::ASTMakeNoneConstant(lineno));

        pList->expressions.emplace_back(fsh::ASTMakeBooleanConstant(lineno, true));

        pList->expressions.emplace_back((ast_t *)(yyvsp[-1]));

        pCall->arguments.reset(pList);

        (yyval) = pCall;
    }
#line 1985 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 320 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTTryCatch *pException = new fsh::ASTTryCatch(lineno);
        pException->try_block.reset((ast_t *)(yyvsp[-3]));
        pException->catch_block.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pException;
    }
#line 1996 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 329 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTIfStatement *pIf = new fsh::ASTIfStatement(lineno);
        pIf->condition.reset((ast_t *)(yyvsp[-5]));
        pIf->if_true.reset((ast_t *)(yyvsp[-3]));
        pIf->if_false.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pIf;
    }
#line 2008 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 339 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFor *pFor = new fsh::ASTFor(lineno);
        pFor->identifier.reset((ast_t *)(yyvsp[-5]));
        pFor->list.reset((ast_t *)(yyvsp[-3]));
        pFor->body.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pFor;
    }
#line 2020 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 349 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTWhile *pWhileIf = new fsh::ASTWhile(lineno);
        pWhileIf->condition.reset((ast_t *)(yyvsp[-3]));
        pWhileIf->if_true.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pWhileIf;
    }
#line 2031 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 358 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionDef *pDef = new fsh::ASTFunctionDef(lineno);
        ast_t *arg3 = (ast_t *)(yyvsp[-3]);
        assert(arg3->type() == fsh::AST_CONSTANT
            || arg3->type() == fsh::AST_IDENTIFIER_LIST);
        if (arg3->type() == fsh::AST_CONSTANT)
        {
            fsh::ASTConstant * id = (fsh::ASTConstant *)(yyvsp[-3]);
            assert(id->ctype == fsh::ASTConstant::CTYPE_IDENTIFIER);
            pDef->arg_names.push_back(id->svalue);
            delete id;
        }
        else
        {
            fsh::ASTIdentifierList *il = (fsh::ASTIdentifierList *)(yyvsp[-3]);
            for (auto& inst : il->identifiers)
            {
                if (inst->type() == fsh::AST_CONSTANT)
                {
                    fsh::ASTConstant *id = (fsh::ASTConstant *)inst.get();
                    assert(id->ctype == fsh::ASTConstant::CTYPE_IDENTIFIER);
                    pDef->arg_names.push_back(id->svalue);
                }
            }
            delete il;
        }
        pDef->functionBody.reset((fsh::Ast *)(yyvsp[-1]));
        (yyval) = pDef;
    }
#line 2065 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 387 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionDef *pDef = new fsh::ASTFunctionDef(lineno);
        pDef->functionBody.reset((fsh::Ast *)(yyvsp[-1]));
        (yyval) = pDef;
    }
#line 2075 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 395 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)(yyvsp[-3]));
        pCall->arguments.reset((fsh::ASTExpressionList *)(yyvsp[-1]));
        //std::cout << "func call " << id->name << std::endl;
        (yyval) = pCall;
    }
#line 2087 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 402 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)(yyvsp[-5]));
        pCall->arguments.reset((fsh::ASTExpressionList *)(yyvsp[-3]));
        pCall->attributes.reset((fsh::ASTExpressionList *)(yyvsp[-1]));
        //std::cout << "func call " << id->name << std::endl;
        (yyval) = pCall;
    }
#line 2100 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 410 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)(yyvsp[-3]));
        pCall->attributes.reset((fsh::ASTExpressionList *)(yyvsp[-1]));
        (yyval) = pCall;
    }
#line 2111 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 416 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTFunctionCall *pCall = new fsh::ASTFunctionCall(lineno);
        pCall->call.reset((ast_t *)(yyvsp[-2]));
        ////std::cout << "func call  " << id->name << std::endl;
        (yyval) = pCall;
    }
#line 2122 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 425 "fsh.y" /* yacc.c:1646  */
    {
        (yyval) = (yyvsp[0]);
    }
#line 2130 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 428 "fsh.y" /* yacc.c:1646  */
    {
        assert(((ast_t *)(yyvsp[0]))->type() == fsh::AST_CONSTANT);
        fsh::ASTConstant *arg3 = (fsh::ASTConstant *)(yyvsp[0]);
        ast_t *ast = (ast_t *)(yyvsp[-2]);
        if (ast->type() == fsh::AST_IDENTIFIER_LIST)
        {
            fsh::ASTIdentifierList *il = (fsh::ASTIdentifierList *)(yyvsp[-2]);
            il->identifiers.emplace_back(arg3);
            (yyval) = il;
        }
        else
        {
            assert(((ast_t *)(yyvsp[-2]))->type() == fsh::AST_CONSTANT);
            fsh::ASTIdentifierList *il = new fsh::ASTIdentifierList(lineno);
            il->identifiers.emplace_back((fsh::ASTConstant *)(yyvsp[-2]));
            il->identifiers.emplace_back(arg3);
            (yyval) = il;
        }
    }
#line 2154 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 450 "fsh.y" /* yacc.c:1646  */
    {
        el_t *el = new el_t(lineno);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2164 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 455 "fsh.y" /* yacc.c:1646  */
    {
        ast_t *ast = (ast_t *)(yyvsp[-2]);
        assert (ast->type() == fsh::AST_EXPRESSION_LIST);
        el_t *el = (el_t *)(yyvsp[-2]);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2176 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 465 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTAssignment *pAttr = new fsh::ASTAssignment(lineno);
        pAttr->lhs.reset((ast_t *)(yyvsp[-2]));
        pAttr->rhs.reset((ast_t *)(yyvsp[0]));
        (yyval) = pAttr;
    }
#line 2187 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 474 "fsh.y" /* yacc.c:1646  */
    {
        el_t *el = new el_t(lineno);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2197 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 479 "fsh.y" /* yacc.c:1646  */
    {
        ast_t *ip = (ast_t *)(yyvsp[-2]);
        assert (ip->type() == fsh::AST_EXPRESSION_LIST);
        el_t *el = (el_t *)(yyvsp[-2]);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2209 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 489 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTExpressionList *el = new fsh::ASTExpressionList(lineno);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2219 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 494 "fsh.y" /* yacc.c:1646  */
    {
        ast_t *ip = (ast_t *)(yyvsp[-2]);
        assert (ip->type() == fsh::AST_EXPRESSION_LIST);
        fsh::ASTExpressionList *el = (el_t *)(yyvsp[-2]);
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2231 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 505 "fsh.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]);}
#line 2237 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 506 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTGlobal *pGlobal = new fsh::ASTGlobal(lineno);
        pGlobal->identifier.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pGlobal;
    }
#line 2247 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 511 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTReturn *pRtn = new fsh::ASTReturn(lineno);
        (yyval) = pRtn;
    }
#line 2256 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 515 "fsh.y" /* yacc.c:1646  */
    {
        fsh::ASTReturn *pRtn = new fsh::ASTReturn(lineno);
        pRtn->expression.reset((ast_t *)(yyvsp[-1]));
        (yyval) = pRtn;
    }
#line 2266 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 525 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 2272 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 529 "fsh.y" /* yacc.c:1646  */
    {(yyval) = (yyvsp[0]);}
#line 2278 "fsh.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 530 "fsh.y" /* yacc.c:1646  */
    {
        ast_t *arg1 = (ast_t *)(yyvsp[-1]);
        fsh::ASTExpressionList *el = nullptr;
        if (arg1->type() != fsh::AST_EXPRESSION_LIST)
        {
            el = new fsh::ASTExpressionList(lineno);
            el->expressions.emplace_back(arg1);
        }
        else
        {
            el = (fsh::ASTExpressionList *)arg1;
        }
        el->expressions.emplace_back((ast_t *)(yyvsp[0]));
        (yyval) = el;
    }
#line 2298 "fsh.tab.c" /* yacc.c:1646  */
    break;


#line 2302 "fsh.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 548 "fsh.y" /* yacc.c:1906  */


extern fsh::Machine machine;

void AstExecute(fsh::Ast *pAst)
{
    fsh::ElementPtr e = machine.Execute(pAst);
    delete pAst;
    if(e->type() == fsh::ELEMENT_TYPE_ERROR)
    {
        fsh::ErrorPtr ep = e.cast<fsh::Error>();
        if (ep->bOk == false && interactive == false)
            std::cout << ep->msg << std::endl;
    }
    if (interactive)
        std::cout << fsh::toString(machine, e, true) << std::endl;
}


