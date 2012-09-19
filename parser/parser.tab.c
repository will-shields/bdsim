/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     EQ = 258,
     GE = 259,
     LE = 260,
     NE = 261,
     UMINUS = 262,
     UPLUS = 263,
     NUMBER = 264,
     VARIABLE = 265,
     VECVAR = 266,
     FUNC = 267,
     STR = 268,
     MARKER = 269,
     ELEMENT = 270,
     DRIFT = 271,
     PCLDRIFT = 272,
     RF = 273,
     DIPOLE = 274,
     RBEND = 275,
     SBEND = 276,
     QUADRUPOLE = 277,
     SEXTUPOLE = 278,
     OCTUPOLE = 279,
     MULTIPOLE = 280,
     SOLENOID = 281,
     COLLIMATOR = 282,
     RCOL = 283,
     ECOL = 284,
     LINE = 285,
     SEQUENCE = 286,
     SPOILER = 287,
     ABSORBER = 288,
     LASER = 289,
     TRANSFORM3D = 290,
     MUSPOILER = 291,
     VKICK = 292,
     HKICK = 293,
     KICK = 294,
     PERIOD = 295,
     APERTURE = 296,
     FILENAME = 297,
     GAS = 298,
     PIPE = 299,
     TUNNEL = 300,
     MATERIAL = 301,
     ATOM = 302,
     BEAM = 303,
     OPTION = 304,
     PRINT = 305,
     RANGE = 306,
     STOP = 307,
     USE = 308,
     VALUE = 309,
     ECHO = 310,
     PRINTF = 311,
     SAMPLE = 312,
     CSAMPLE = 313,
     BETA0 = 314,
     TWISS = 315,
     DUMP = 316,
     IF = 317,
     ELSE = 318,
     BEGN = 319,
     END = 320,
     FOR = 321,
     CUT = 322
   };
#endif
/* Tokens.  */
#define EQ 258
#define GE 259
#define LE 260
#define NE 261
#define UMINUS 262
#define UPLUS 263
#define NUMBER 264
#define VARIABLE 265
#define VECVAR 266
#define FUNC 267
#define STR 268
#define MARKER 269
#define ELEMENT 270
#define DRIFT 271
#define PCLDRIFT 272
#define RF 273
#define DIPOLE 274
#define RBEND 275
#define SBEND 276
#define QUADRUPOLE 277
#define SEXTUPOLE 278
#define OCTUPOLE 279
#define MULTIPOLE 280
#define SOLENOID 281
#define COLLIMATOR 282
#define RCOL 283
#define ECOL 284
#define LINE 285
#define SEQUENCE 286
#define SPOILER 287
#define ABSORBER 288
#define LASER 289
#define TRANSFORM3D 290
#define MUSPOILER 291
#define VKICK 292
#define HKICK 293
#define KICK 294
#define PERIOD 295
#define APERTURE 296
#define FILENAME 297
#define GAS 298
#define PIPE 299
#define TUNNEL 300
#define MATERIAL 301
#define ATOM 302
#define BEAM 303
#define OPTION 304
#define PRINT 305
#define RANGE 306
#define STOP 307
#define USE 308
#define VALUE 309
#define ECHO 310
#define PRINTF 311
#define SAMPLE 312
#define CSAMPLE 313
#define BETA0 314
#define TWISS 315
#define DUMP 316
#define IF 317
#define ELSE 318
#define BEGN 319
#define END 320
#define FOR 321
#define CUT 322




/* Copy the first part of user declarations.  */
#line 7 "parser.y"


  extern int line_num;
  extern char* yyfilename;

  const int ECHO_GRAMMAR = 0; // print grammar rule expansion (for debugging)
  const int VERBOSE = 0; // print warnings and errors
  const int VERBOSE_EXPAND = 0; // print the process of line expansion 
  const int INTERACTIVE = 0; // print output of commands (like in interactive mode)

#include "parser.h"

  int execute = 1;
  int element_count = 1; // for samplers , ranges etc.
#ifdef __cplusplus
  using namespace std;
#endif



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 28 "parser.y"
{
  double dval;
  int ival;
  struct symtab *symp;
  char *str;
  struct Array *array;
}
/* Line 193 of yacc.c.  */
#line 258 "parser.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 271 "parser.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   570

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  187
/* YYNRULES -- Number of states.  */
#define YYNSTATES  372

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      78,    79,     5,     3,    81,     4,     2,     6,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    80,    75,
       8,    82,     9,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    83,     2,    84,     7,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,    77,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    13,    22,    24,    28,    33,
      34,    36,    38,    40,    42,    46,    50,    54,    58,    62,
      66,    70,    74,    78,    82,    86,    90,    94,    98,   102,
     106,   110,   114,   118,   122,   126,   130,   134,   138,   142,
     146,   148,   152,   156,   160,   164,   168,   172,   176,   180,
     184,   188,   192,   196,   200,   204,   208,   212,   216,   220,
     224,   228,   232,   234,   235,   241,   247,   251,   255,   261,
     265,   271,   278,   284,   285,   289,   295,   301,   303,   307,
     311,   316,   319,   320,   324,   330,   336,   338,   342,   346,
     351,   354,   355,   359,   365,   371,   373,   377,   381,   383,
     385,   387,   389,   391,   396,   400,   404,   408,   412,   416,
     419,   422,   426,   432,   436,   440,   444,   448,   452,   456,
     461,   465,   469,   473,   475,   477,   479,   483,   487,   491,
     495,   499,   503,   507,   511,   515,   519,   520,   524,   526,
     527,   531,   533,   535,   539,   541,   545,   549,   553,   557,
     561,   565,   568,   572,   576,   580,   584,   588,   592,   596,
     598,   602,   612,   616,   623,   627,   631,   637,   643,   647,
     649,   653,   657,   663,   669,   677,   683,   689,   693,   694,
     700,   704,   710,   714,   715,   721,   725,   731
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      86,     0,    -1,    -1,    86,    87,    75,    -1,    88,    76,
      87,    77,    -1,    88,    76,    87,    77,    70,    76,    87,
      77,    -1,    89,    -1,    71,    86,    72,    -1,    69,    78,
     121,    79,    -1,    -1,   120,    -1,   128,    -1,    90,    -1,
       1,    -1,    17,    80,    91,    -1,    17,    80,    92,    -1,
      17,    80,    93,    -1,    17,    80,    94,    -1,    17,    80,
      95,    -1,    17,    80,    96,    -1,    17,    80,    97,    -1,
      17,    80,    98,    -1,    17,    80,    99,    -1,    17,    80,
     100,    -1,    17,    80,   101,    -1,    17,    80,   102,    -1,
      17,    80,   103,    -1,    17,    80,   106,    -1,    17,    80,
     104,    -1,    17,    80,   105,    -1,    17,    80,   109,    -1,
      17,    80,   107,    -1,    17,    80,   108,    -1,    17,    80,
     115,    -1,    17,    80,   116,    -1,    17,    80,   112,    -1,
      17,    80,   113,    -1,    17,    81,   114,    -1,    17,    80,
     110,    -1,    17,    80,   111,    -1,    21,    -1,    23,    81,
     114,    -1,    24,    81,   114,    -1,    25,    81,   114,    -1,
      28,    81,   114,    -1,    27,    81,   114,    -1,    44,    81,
     114,    -1,    45,    81,   114,    -1,    29,    81,   114,    -1,
      30,    81,   114,    -1,    31,    81,   114,    -1,    32,    81,
     114,    -1,    33,    81,   114,    -1,    36,    81,   114,    -1,
      43,    81,   114,    -1,    35,    81,   114,    -1,    41,    81,
     114,    -1,    42,    81,   114,    -1,    22,    81,   114,    -1,
      53,    81,   114,    -1,    54,    81,   114,    -1,    17,    81,
     114,    -1,    17,    -1,    -1,    17,    82,   121,    81,   114,
      -1,    17,    82,   123,    81,   114,    -1,    17,    82,   123,
      -1,    17,    82,   121,    -1,    17,    82,    20,    81,   114,
      -1,    17,    82,    20,    -1,    37,    82,    78,   117,    79,
      -1,    37,    82,     4,    78,   118,    79,    -1,    38,    82,
      78,   119,    79,    -1,    -1,    17,    81,   117,    -1,    17,
       5,    16,    81,   117,    -1,    16,     5,    17,    81,   117,
      -1,    17,    -1,    17,     5,    16,    -1,    16,     5,    17,
      -1,     4,    17,    81,   117,    -1,     4,    17,    -1,    -1,
      17,    81,   118,    -1,    17,     5,    16,    81,   118,    -1,
      16,     5,    17,    81,   118,    -1,    17,    -1,    17,     5,
      16,    -1,    16,     5,    17,    -1,     4,    17,    81,   117,
      -1,     4,    17,    -1,    -1,    17,    81,   119,    -1,    17,
       5,    16,    81,   119,    -1,    16,     5,    17,    81,   119,
      -1,    17,    -1,    17,     5,    16,    -1,    16,     5,    17,
      -1,   121,    -1,   123,    -1,   122,    -1,    16,    -1,    17,
      -1,    19,    78,   121,    79,    -1,   121,     3,   121,    -1,
     121,     4,   121,    -1,   121,     5,   121,    -1,   121,     6,
     121,    -1,   121,     7,   121,    -1,     4,   121,    -1,     3,
     121,    -1,    78,   121,    79,    -1,     8,   123,    81,   123,
       9,    -1,   121,     8,   121,    -1,   121,    12,   121,    -1,
     121,     9,   121,    -1,   121,    11,   121,    -1,   121,    13,
     121,    -1,   121,    10,   121,    -1,    17,    83,    17,    84,
      -1,    17,    82,   121,    -1,    17,    82,   123,    -1,    18,
      82,   123,    -1,    18,    -1,   124,    -1,   125,    -1,   123,
       3,   123,    -1,   123,     4,   123,    -1,   123,     3,   121,
      -1,   123,     5,   121,    -1,   123,     6,   121,    -1,   121,
       3,   123,    -1,   121,     4,   123,    -1,   121,     5,   123,
      -1,    76,   126,    77,    -1,    83,   127,    84,    -1,    -1,
     121,    81,   126,    -1,   121,    -1,    -1,    20,    81,   127,
      -1,    20,    -1,    59,    -1,    55,    81,   134,    -1,    57,
      -1,    57,    81,    37,    -1,    57,    81,    56,    -1,    57,
      81,    17,    -1,    57,    81,    18,    -1,    60,    81,   129,
      -1,    56,    81,   133,    -1,    62,    20,    -1,    64,    81,
     130,    -1,    65,    81,   131,    -1,    50,    81,   132,    -1,
      52,    81,   114,    -1,    66,    81,   133,    -1,    67,    81,
     133,    -1,    68,    81,   130,    -1,    17,    -1,    47,    82,
      17,    -1,    47,    82,    17,    81,    58,    82,    17,     6,
      17,    -1,    58,    82,    17,    -1,    58,    82,    17,    83,
      16,    84,    -1,    17,    82,   121,    -1,    17,    82,    20,
      -1,    17,    82,   121,    81,   131,    -1,    17,    82,    20,
      81,   131,    -1,   130,    81,   131,    -1,   130,    -1,    17,
      82,   121,    -1,    17,    82,    20,    -1,    17,    82,   121,
      81,   132,    -1,    17,    82,    20,    81,   132,    -1,    58,
      82,    17,     6,    17,    81,   132,    -1,    58,    82,    17,
       6,    17,    -1,    58,    82,    17,    81,   132,    -1,    58,
      82,    17,    -1,    -1,    17,    82,   121,    81,   133,    -1,
      17,    82,   121,    -1,    17,    82,    20,    81,   133,    -1,
      17,    82,    20,    -1,    -1,    17,    82,   121,    81,   134,
      -1,    17,    82,   121,    -1,    17,    82,    20,    81,   134,
      -1,    17,    82,    20,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    67,    67,    68,    79,    80,    82,    83,    86,    89,
      90,    91,    92,    93,   102,   111,   120,   129,   138,   147,
     157,   166,   175,   185,   195,   205,   215,   225,   235,   245,
     255,   265,   275,   285,   298,   308,   320,   332,   357,   366,
     377,   379,   382,   385,   388,   391,   394,   397,   400,   403,
     406,   409,   412,   415,   418,   421,   424,   427,   430,   433,
     436,   439,   462,   485,   486,   594,   653,   712,   802,   865,
     929,   932,   941,   945,   946,   963,   981,   999,  1016,  1034,
    1052,  1069,  1088,  1089,  1106,  1124,  1142,  1159,  1177,  1195,
    1212,  1231,  1232,  1249,  1267,  1285,  1302,  1320,  1340,  1348,
    1361,  1382,  1383,  1388,  1389,  1390,  1391,  1392,  1393,  1394,
    1395,  1396,  1397,  1412,  1413,  1414,  1415,  1416,  1417,  1418,
    1425,  1438,  1452,  1466,  1482,  1502,  1515,  1538,  1561,  1582,
    1602,  1622,  1642,  1662,  1685,  1713,  1729,  1730,  1735,  1742,
    1743,  1748,  1755,  1756,  1757,  1758,  1759,  1760,  1768,  1783,
    1784,  1785,  1786,  1796,  1807,  1817,  1826,  1833,  1841,  1855,
    1865,  1875,  1889,  1895,  1906,  1918,  1926,  1939,  1945,  1950,
    1957,  1969,  1982,  1995,  2007,  2012,  2017,  2022,  2029,  2030,
    2035,  2040,  2045,  2052,  2053,  2058,  2063,  2068
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'+'", "'-'", "'*'", "'/'", "'^'", "'<'",
  "'>'", "EQ", "GE", "LE", "NE", "UMINUS", "UPLUS", "NUMBER", "VARIABLE",
  "VECVAR", "FUNC", "STR", "MARKER", "ELEMENT", "DRIFT", "PCLDRIFT", "RF",
  "DIPOLE", "RBEND", "SBEND", "QUADRUPOLE", "SEXTUPOLE", "OCTUPOLE",
  "MULTIPOLE", "SOLENOID", "COLLIMATOR", "RCOL", "ECOL", "LINE",
  "SEQUENCE", "SPOILER", "ABSORBER", "LASER", "TRANSFORM3D", "MUSPOILER",
  "VKICK", "HKICK", "KICK", "PERIOD", "APERTURE", "FILENAME", "GAS",
  "PIPE", "TUNNEL", "MATERIAL", "ATOM", "BEAM", "OPTION", "PRINT", "RANGE",
  "STOP", "USE", "VALUE", "ECHO", "PRINTF", "SAMPLE", "CSAMPLE", "BETA0",
  "TWISS", "DUMP", "IF", "ELSE", "BEGN", "END", "FOR", "CUT", "';'", "'{'",
  "'}'", "'('", "')'", "':'", "','", "'='", "'['", "']'", "$accept",
  "input", "stmt", "if_clause", "atomic_stmt", "decl", "marker", "drift",
  "pcldrift", "rf", "sbend", "rbend", "vkick", "hkick", "quad",
  "sextupole", "octupole", "multipole", "solenoid", "ecol", "muspoiler",
  "rcol", "laser", "transform3d", "element", "matdef", "atom", "extension",
  "newinstance", "parameters", "line", "sequence", "element_seq",
  "rev_element_seq", "seq_element_seq", "expr", "aexpr", "assignment",
  "vecexpr", "vectnum", "vectstr", "numbers", "letters", "command",
  "use_parameters", "sample_options", "csample_options", "gas_options",
  "option_parameters", "beam_parameters", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    43,    45,    42,    47,    94,    60,    62,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,    59,   123,   125,    40,    41,
      58,    44,    61,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    87,    87,    88,    89,
      89,    89,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   116,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   119,   119,   119,   119,   119,   119,   119,   120,   120,
     120,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     122,   122,   122,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   124,   125,   126,   126,   126,   127,
     127,   127,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   129,
     129,   129,   130,   130,   131,   131,   131,   131,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   132,   133,   133,
     133,   133,   133,   134,   134,   134,   134,   134
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     4,     8,     1,     3,     4,     0,
       1,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     0,     5,     5,     3,     3,     5,     3,
       5,     6,     5,     0,     3,     5,     5,     1,     3,     3,
       4,     2,     0,     3,     5,     5,     1,     3,     3,     4,
       2,     0,     3,     5,     5,     1,     3,     3,     1,     1,
       1,     1,     1,     4,     3,     3,     3,     3,     3,     2,
       2,     3,     5,     3,     3,     3,     3,     3,     3,     4,
       3,     3,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     0,     3,     1,     0,
       3,     1,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     9,     3,     6,     3,     3,     5,     5,     3,     1,
       3,     3,     5,     5,     7,     5,     5,     3,     0,     5,
       3,     5,     3,     0,     5,     3,     5,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,    13,     0,     0,     0,   101,   102,   123,
       0,     0,     0,     0,     0,   144,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     2,   136,     0,   139,     0,
       0,     6,    12,    10,    98,   100,    99,   124,   125,    11,
     102,   110,   109,   123,     0,     0,     0,    63,     0,     0,
       0,     0,     0,    63,   183,   178,     0,     0,   151,     0,
       0,   178,   178,     0,     0,     0,   138,     0,     0,   141,
       0,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    40,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    28,
      29,    27,    31,    32,    30,    38,    39,    35,    36,    33,
      34,     0,    37,   120,   121,     0,   122,     0,     0,     0,
     154,   155,     0,   143,     0,   150,   147,   148,   145,   146,
     159,     0,   149,     0,   152,     0,   169,   153,   156,   157,
     158,     0,     7,   136,   134,   111,   139,   135,     0,   104,
     131,   105,   132,   106,   133,   107,   108,   113,   115,   118,
     116,   114,   117,   128,   126,   127,   129,   130,   104,   105,
     106,     0,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,     0,     0,    63,    63,
      63,    63,    63,    63,    63,     0,   119,   103,     0,     0,
       0,     0,     0,     0,     0,     0,     8,   137,   140,     4,
     112,    61,    58,    41,    42,    43,    45,    44,    48,    49,
      50,    51,    52,    55,    53,     0,    73,    91,    56,    57,
      54,    46,    47,    59,    60,    69,    67,    66,   171,   170,
     177,   187,   185,   182,   180,   160,   162,   165,   164,   168,
       0,    82,     0,     0,    77,     0,     0,    95,     0,    63,
      63,    63,     0,     0,     0,     0,   183,   183,   178,   178,
       0,     0,     0,     0,     0,     0,     0,    86,     0,    81,
       0,     0,    73,    70,     0,     0,    91,    72,    68,    64,
      65,   173,   172,   175,   176,   186,   184,   181,   179,     0,
       0,   167,   166,     0,    90,     0,     0,    82,    71,    73,
      79,    78,    74,    97,    96,    92,     0,     0,   163,     5,
      73,    88,    87,    83,    80,    73,    73,    91,    91,   174,
       0,    89,    82,    82,    76,    75,    94,    93,     0,    85,
      84,   161
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    29,    30,    31,    32,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   142,
     139,   140,   285,   308,   288,    33,    34,    35,    36,    37,
      38,    67,    70,    39,   162,   166,   167,   150,   155,   153
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -286
static const yytype_int16 yypact[] =
{
    -286,   122,  -286,  -286,   453,   453,   128,  -286,   197,   -35,
     -29,   -23,   -16,   -11,    -9,    17,  -286,    21,    85,    27,
      30,    32,    34,    43,    42,  -286,   453,   453,   109,    59,
      61,  -286,  -286,  -286,   504,  -286,   310,  -286,  -286,  -286,
      60,  -286,  -286,  -286,   504,    14,   505,   118,   128,   125,
     128,   453,    49,   118,   131,   158,   146,    -1,  -286,   127,
      51,   158,   158,   127,   453,   191,    25,    99,   389,   115,
      96,  -286,   267,   128,   128,   128,   453,   453,   453,   453,
     453,   453,   453,   453,   128,   128,   453,   453,   453,   453,
     453,   128,   120,  -286,   142,   143,   145,   148,   151,   154,
     161,   164,   168,   173,   180,   192,   200,   190,   205,   201,
     237,   239,   240,   244,   247,   249,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,   255,  -286,   504,   310,   119,   310,   400,   257,   258,
    -286,  -286,   259,  -286,   260,  -286,  -286,  -286,  -286,  -286,
    -286,   264,  -286,   265,  -286,   266,   268,  -286,  -286,  -286,
    -286,   411,  -286,   453,  -286,  -286,   109,  -286,   150,   475,
       6,   475,     6,   544,  -286,   544,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,   475,     6,     6,   544,   544,   486,   486,
     544,   368,   118,   118,   118,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,    39,   286,   118,   118,
     118,   118,   118,   118,   118,    36,  -286,  -286,   359,   349,
     422,   428,   351,   353,   446,    51,  -286,  -286,  -286,   295,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,   350,   248,    -2,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,   346,   209,    18,   354,   288,
      35,   362,   299,   365,   348,   370,   357,   371,   378,  -286,
     358,   272,   436,   450,    37,   379,   459,    40,   388,   118,
     118,   118,    49,    49,   456,    49,   131,   131,   158,   158,
     416,   462,    51,    51,   267,   458,   484,    52,   423,   420,
     487,   489,   248,  -286,   501,   503,    -2,  -286,  -286,  -286,
    -286,  -286,  -286,   439,  -286,  -286,  -286,  -286,  -286,   421,
     437,  -286,  -286,   448,   442,   522,   528,   272,  -286,   248,
     464,   479,  -286,   480,   481,  -286,    49,   546,  -286,  -286,
     248,   483,   485,  -286,  -286,   248,   248,    -2,    -2,  -286,
     559,  -286,   272,   272,  -286,  -286,  -286,  -286,   550,  -286,
    -286,  -286
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -286,   543,   -71,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,
    -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,  -286,   -53,
    -286,  -286,   121,  -266,  -257,  -286,     0,  -286,    19,  -286,
    -286,   396,   394,  -286,  -286,   -50,  -175,  -285,   -59,  -234
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -10
static const yytype_int16 yytable[] =
{
     151,   178,   168,   169,    41,    42,    44,   321,   322,   164,
     324,    86,    87,   170,   286,   287,   160,    84,    85,    86,
      87,    84,    85,    86,    87,    45,    66,    68,    88,    89,
      90,    76,    77,    78,    79,    80,    81,    82,    83,     4,
       5,   294,   311,   255,     6,   315,   161,    50,   143,    51,
      44,   147,     7,    40,    43,    10,   265,   336,    52,   345,
     279,   359,   325,   326,   171,    53,   148,   144,   165,   146,
      54,   353,    55,   179,   181,   183,   185,   186,   187,   188,
     189,   190,   191,   192,   193,    44,   196,   197,   198,   199,
     200,    44,   180,   182,   184,    91,   369,   370,    56,   291,
     366,   367,    57,   194,   195,    58,   173,   149,    59,   163,
     201,    60,    26,    61,    27,    62,   295,   256,   312,    28,
      64,   316,     2,     3,    63,     4,     5,   331,   332,    69,
       6,     4,     5,   337,    71,   141,     6,    72,     7,     8,
       9,    10,   145,    49,     7,    40,    43,    10,   152,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   156,   157,   258,   259,   260,   261,   262,
     263,   264,    11,    66,    12,   154,   174,    13,    14,    15,
     177,    16,    17,   158,    18,   163,    19,    20,    21,    22,
      23,    24,     3,    25,     4,     5,   176,    -9,    26,     6,
      27,   202,   159,   226,    26,    28,    27,     7,     8,     9,
      10,    28,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,   203,   204,   266,   205,   239,   269,   206,
     272,   274,   207,   333,   278,   208,   318,   319,   320,   327,
     328,    11,   209,    12,   267,   210,    13,    14,    15,   211,
      16,    17,   282,    18,   212,    19,    20,    21,    22,    23,
      24,   213,    25,   172,   283,   284,    -9,    26,     3,    27,
       4,     5,   216,   214,    28,     6,   305,    46,    47,    48,
      49,   215,   218,     7,     8,     9,    10,   217,   306,   307,
     290,    88,    89,    90,    76,    77,    78,    79,    80,    81,
      82,    83,    88,    89,    90,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    11,   219,    12,
     220,   221,    13,    14,    15,   222,    16,    17,   223,    18,
     224,    19,    20,    21,    22,    23,    24,   225,    25,   228,
     229,   230,   231,    26,    -9,    27,   232,   233,   234,   235,
      28,    88,    89,    90,    76,    77,    78,    79,    80,    81,
      82,    83,     4,     5,   257,   280,   270,     6,   275,   293,
     276,    84,    85,    86,    87,     7,    40,   240,    10,   268,
     297,    88,    89,    90,    76,    77,    78,    79,    80,    81,
      82,    83,    88,    89,    90,    76,    77,    78,    79,    80,
      81,    82,    83,    88,    89,    90,    76,    77,    78,    79,
      80,    81,    82,    83,    88,    89,    90,    76,    77,    78,
      79,    80,    81,    82,    83,     4,     5,   289,   281,   299,
       6,     4,     5,   342,   304,   292,     6,    27,     7,    40,
     301,    10,   271,   296,     7,    40,   298,    10,   273,     4,
       5,   300,   302,   309,     6,   310,     4,     5,   313,   303,
     354,     6,     7,    40,   314,    10,   277,   317,   175,     7,
      40,   361,    10,   323,   329,   334,   364,   365,   330,   227,
      75,    76,    77,    78,    79,    80,    81,    82,    83,   335,
     236,    90,    76,    77,    78,    79,    80,    81,    82,    83,
      27,   339,   338,   347,   340,   341,    27,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   343,   344,
     346,   348,    92,   350,    27,   349,    93,    94,    95,    96,
      97,    27,    98,    99,   100,   101,   102,   103,   104,   351,
     105,   106,   107,   108,   352,   355,   109,   110,   111,   112,
     113,    77,    78,    79,    80,    81,    82,    83,   114,   115,
     356,   357,   358,   360,   362,   368,   363,   371,    65,   237,
     238
};

static const yytype_uint16 yycheck[] =
{
      53,    72,    61,    62,     4,     5,     6,   292,   293,    59,
     295,     5,     6,    63,    16,    17,    17,     3,     4,     5,
       6,     3,     4,     5,     6,     6,    26,    27,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     3,
       4,     6,     5,     4,     8,     5,    47,    82,    48,    78,
      50,    51,    16,    17,    18,    19,    20,     5,    81,   316,
     235,   346,   296,   297,    64,    81,    17,    48,    17,    50,
      81,   337,    81,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    73,    74,    75,    81,   362,   363,    81,    81,
     357,   358,    81,    84,    85,    20,    81,    58,    81,    58,
      91,    81,    76,    81,    78,    81,    81,    78,    81,    83,
      78,    81,     0,     1,    81,     3,     4,   302,   303,    20,
       8,     3,     4,    81,    75,    17,     8,    76,    16,    17,
      18,    19,    17,    83,    16,    17,    18,    19,    17,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,    17,    18,   218,   219,   220,   221,   222,
     223,   224,    50,   173,    52,    17,    77,    55,    56,    57,
      84,    59,    60,    37,    62,    58,    64,    65,    66,    67,
      68,    69,     1,    71,     3,     4,    81,    75,    76,     8,
      78,    81,    56,    84,    76,    83,    78,    16,    17,    18,
      19,    83,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    81,    81,   225,    81,    77,   228,    81,
     230,   231,    81,   304,   234,    81,   289,   290,   291,   298,
     299,    50,    81,    52,   225,    81,    55,    56,    57,    81,
      59,    60,     4,    62,    81,    64,    65,    66,    67,    68,
      69,    81,    71,    72,    16,    17,    75,    76,     1,    78,
       3,     4,    82,    81,    83,     8,     4,    80,    81,    82,
      83,    81,    81,    16,    17,    18,    19,    82,    16,    17,
      81,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     3,     4,     5,     6,    50,    81,    52,
      81,    81,    55,    56,    57,    81,    59,    60,    81,    62,
      81,    64,    65,    66,    67,    68,    69,    82,    71,    82,
      82,    82,    82,    76,    77,    78,    82,    82,    82,    81,
      83,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     3,     4,    78,    70,    17,     8,    17,    81,
      17,     3,     4,     5,     6,    16,    17,     9,    19,    20,
      81,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     3,     4,    81,    78,    81,
       8,     3,     4,   312,    76,    81,     8,    78,    16,    17,
      83,    19,    20,    81,    16,    17,    81,    19,    20,     3,
       4,    81,    81,    17,     8,     5,     3,     4,    79,    81,
     339,     8,    16,    17,     5,    19,    20,    79,    79,    16,
      17,   350,    19,    17,    58,    17,   355,   356,    16,    79,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     5,
      79,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      78,    81,    79,    82,    17,    16,    78,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    17,    16,
      81,    84,    17,    81,    78,    77,    21,    22,    23,    24,
      25,    78,    27,    28,    29,    30,    31,    32,    33,    17,
      35,    36,    37,    38,    16,    81,    41,    42,    43,    44,
      45,     7,     8,     9,    10,    11,    12,    13,    53,    54,
      81,    81,    81,    17,    81,     6,    81,    17,    25,   173,
     176
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    86,     0,     1,     3,     4,     8,    16,    17,    18,
      19,    50,    52,    55,    56,    57,    59,    60,    62,    64,
      65,    66,    67,    68,    69,    71,    76,    78,    83,    87,
      88,    89,    90,   120,   121,   122,   123,   124,   125,   128,
      17,   121,   121,    18,   121,   123,    80,    81,    82,    83,
      82,    78,    81,    81,    81,    81,    81,    81,    20,    81,
      81,    81,    81,    81,    78,    86,   121,   126,   121,    20,
     127,    75,    76,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     3,     4,     5,     6,     3,     4,
       5,    81,    17,    21,    22,    23,    24,    25,    27,    28,
      29,    30,    31,    32,    33,    35,    36,    37,    38,    41,
      42,    43,    44,    45,    53,    54,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   115,
     116,    17,   114,   121,   123,    17,   123,   121,    17,    58,
     132,   114,    17,   134,    17,   133,    17,    18,    37,    56,
      17,    47,   129,    58,   130,    17,   130,   131,   133,   133,
     130,   121,    72,    81,    77,    79,    81,    84,    87,   121,
     123,   121,   123,   121,   123,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   123,   123,   121,   121,   121,   121,
     121,   123,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    82,    82,    81,    81,
      81,    81,    81,    81,    81,    82,    84,    79,    82,    82,
      82,    82,    82,    82,    82,    81,    79,   126,   127,    77,
       9,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,     4,    78,    78,   114,   114,
     114,   114,   114,   114,   114,    20,   121,   123,    20,   121,
      17,    20,   121,    20,   121,    17,    17,    20,   121,   131,
      70,    78,     4,    16,    17,   117,    16,    17,   119,    81,
      81,    81,    81,    81,     6,    81,    81,    81,    81,    81,
      81,    83,    81,    81,    76,     4,    16,    17,   118,    17,
       5,     5,    81,    79,     5,     5,    81,    79,   114,   114,
     114,   132,   132,    17,   132,   134,   134,   133,   133,    58,
      16,   131,   131,    87,    17,     5,     5,    81,    79,    81,
      17,    16,   117,    17,    16,   119,    81,    82,    84,    77,
      81,    17,    16,   118,   117,    81,    81,    81,    81,   132,
      17,   117,    81,    81,   117,   117,   119,   119,     6,   118,
     118,    17
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 69 "parser.y"
    { 
	 if(ECHO_GRAMMAR) printf("input -> input stmt ';' \n");
       ;}
    break;

  case 4:
#line 79 "parser.y"
    { if(ECHO_GRAMMAR) printf("stmt -> IF '(' aexpr ')' stmt\n" ); execute = 1;;}
    break;

  case 5:
#line 81 "parser.y"
    { if(ECHO_GRAMMAR) printf("stmt -> IF '(' bool_expr ')' ELSE stmt \n" ); ;}
    break;

  case 6:
#line 82 "parser.y"
    { if(ECHO_GRAMMAR) printf("stmt -> atomic_stmt \n"); ;}
    break;

  case 7:
#line 83 "parser.y"
    { if(ECHO_GRAMMAR) printf("stmt -> '{' stmt ';' atomic_stmt '}' \n"); ;}
    break;

  case 8:
#line 86 "parser.y"
    {if( ((yyvsp[(3) - (4)].dval) > 0) && (execute > 0) ) execute = 1; else execute = 0;;}
    break;

  case 10:
#line 90 "parser.y"
    { if(ECHO_GRAMMAR) printf("atomic_stmt -> expr\n"); ;}
    break;

  case 11:
#line 91 "parser.y"
    { if(ECHO_GRAMMAR) printf("atomic_stmt -> command\n"); ;}
    break;

  case 12:
#line 92 "parser.y"
    { if(ECHO_GRAMMAR) printf("atomic_stmt -> decl\n"); ;}
    break;

  case 13:
#line 94 "parser.y"
    {
		//yyerror(" : some error message\n"); 
		if(ECHO_GRAMMAR) printf("\natomic_stmt -> error\n");
	      ;}
    break;

  case 14:
#line 103 "parser.y"
    {
	 if(execute)  {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : marker\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_MARKER);
	   params.flush();
	 }
       ;}
    break;

  case 15:
#line 112 "parser.y"
    {
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : drift\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_DRIFT);
	   params.flush();
	 }
       ;}
    break;

  case 16:
#line 121 "parser.y"
    {
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : pcldrift\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_PCLDRIFT);
	   params.flush();
	 }
       ;}
    break;

  case 17:
#line 130 "parser.y"
    {
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : rf\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_RF);
	   params.flush();
	 }
       ;}
    break;

  case 18:
#line 139 "parser.y"
    {  
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : sbend\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_SBEND);
	   params.flush();
	 }
       ;}
    break;

  case 19:
#line 148 "parser.y"
    {
         if(execute) {
           if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : rbend\n",(yyvsp[(1) - (3)].symp)->name);
           // check parameters and write into element table
           write_table(params,(yyvsp[(1) - (3)].symp)->name,_RBEND);
           params.flush();
         }
       ;}
    break;

  case 20:
#line 158 "parser.y"
    {  
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : vkick\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_VKICK);
	   params.flush();
	 }
       ;}
    break;

  case 21:
#line 167 "parser.y"
    {  
	 if(execute) {
	   if(ECHO_GRAMMAR) printf("decl -> VARIABLE (%s) : hkick\n",(yyvsp[(1) - (3)].symp)->name);
	   // check parameters and write into element table
	   write_table(params,(yyvsp[(1) - (3)].symp)->name,_HKICK);
	   params.flush();
	 }
       ;}
    break;

  case 22:
#line 176 "parser.y"
    {
	 if(execute)       
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : quad %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_QUAD);
	     params.flush();
	   }
       ;}
    break;

  case 23:
#line 186 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : sext %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_SEXTUPOLE);
	     params.flush();
	   }
       ;}
    break;

  case 24:
#line 196 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("VARIABLE : octupole %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_OCTUPOLE);
	     params.flush();
	   }
       ;}
    break;

  case 25:
#line 206 "parser.y"
    {
	 if(execute)
	   {	 
	     if(ECHO_GRAMMAR) printf("VARIABLE : multipole %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_MULT);
	     params.flush();	 
	   }
       ;}
    break;

  case 26:
#line 216 "parser.y"
    {
	 if(execute)       
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : solenoid %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_SOLENOID);
	     params.flush();
	   }
       ;}
    break;

  case 27:
#line 226 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("VARIABLE : rcol %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_RCOL);
	     params.flush();
	   }
       ;}
    break;

  case 28:
#line 236 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("VARIABLE : ecol %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_ECOL);
	     params.flush();
	   }
       ;}
    break;

  case 29:
#line 246 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("VARIABLE : muspoiler %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_MUSPOILER);
	     params.flush();
	   }
       ;}
    break;

  case 30:
#line 256 "parser.y"
    {
	 if(execute)
	   {	 
	     if(ECHO_GRAMMAR) printf("VARIABLE : element %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_ELEMENT);
	     params.flush();	 
	   }
       ;}
    break;

  case 31:
#line 266 "parser.y"
    {
	 if(execute)
	   {	 
	     if(ECHO_GRAMMAR) printf("VARIABLE : laser %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_LASER);
	     params.flush();	 
	   }
       ;}
    break;

  case 32:
#line 276 "parser.y"
    {
	 if(execute)
	   {	 
	     if(ECHO_GRAMMAR) printf("VARIABLE : transform3d %s \n",(yyvsp[(1) - (3)].symp)->name);
	     // check parameters and write into element table
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_TRANSFORM3D);
	     params.flush();	 
	   }
       ;}
    break;

  case 33:
#line 286 "parser.y"
    {
	 if(execute)
	   {
	     // create entry in the main table and add pointer to the parsed sequence
	     if(ECHO_GRAMMAR) printf("VARIABLE : LINE %s\n",(yyvsp[(1) - (3)].symp)->name);
	     //  list<struct Element>* tmp_list = new list<struct Element>;
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_LINE,new list<struct Element>(tmp_list));
	     // write_table(params,$1->name,_LINE,tmp_list);
	      tmp_list.erase(tmp_list.begin(), tmp_list.end());
	      tmp_list.~list<struct Element>();
	   }
       ;}
    break;

  case 34:
#line 299 "parser.y"
    {
	 if(execute)
	   {
             // create entry in the main table and add pointer to the parsed sequence
	     if(ECHO_GRAMMAR) printf("VARIABLE : SEQUENCE %s\n",(yyvsp[(1) - (3)].symp)->name);
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_SEQUENCE,new list<struct Element>(tmp_list));
	     tmp_list.erase(tmp_list.begin(), tmp_list.end());
	   }
       ;}
    break;

  case 35:
#line 309 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : VARIABLE, %s  :  %s\n",(yyvsp[(1) - (3)].symp)->name, typestr((yyvsp[(3) - (3)].ival)));
	     if((yyvsp[(3) - (3)].ival) != _NONE)
	       {
		 write_table(params,(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].ival));
	       }
	     params.flush();
	   }
       ;}
    break;

  case 36:
#line 321 "parser.y"
    {
         if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : VARIABLE, %s  :  %s\n",(yyvsp[(1) - (3)].symp)->name, typestr((yyvsp[(3) - (3)].ival)));
	     if((yyvsp[(3) - (3)].ival) != _NONE)
	       {
		 write_table(params,(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].ival));
	       }
	     params.flush();
	   }
       ;}
    break;

  case 37:
#line 333 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("edit : VARIABLE parameters   -- %s \n",(yyvsp[(1) - (3)].symp)->name);
	     list<struct Element>::iterator it = element_lookup((yyvsp[(1) - (3)].symp)->name);
	     list<struct Element>::iterator iterNULL = element_list.end();
	     if(it == iterNULL)
	       {
		 if(VERBOSE) printf("type %s has not been defined\n",(yyvsp[(1) - (3)].symp)->name);
	       }
	     else
	       {
		 // inherit properties from the base type
		 inherit_properties(*it);
	       }
		
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : VARIABLE, %s  :  %s\n",(yyvsp[(1) - (3)].symp)->name, typestr((*it).type));
	     if((*it).type != _NONE)
	       {
		 write_table(params,(yyvsp[(1) - (3)].symp)->name,(*it).type);
	       }
	     params.flush();
	   }
       ;}
    break;

  case 38:
#line 358 "parser.y"
    {
	 if(execute)
	   {
	     if(ECHO_GRAMMAR) printf("decl -> VARIABLE : Material, %s \n",(yyvsp[(1) - (3)].symp)->name);
	     write_table(params,(yyvsp[(1) - (3)].symp)->name,_MATERIAL);
	     params.flush();
	   }
       ;}
    break;

  case 39:
#line 367 "parser.y"
    {
         if(execute)
           {
             if(ECHO_GRAMMAR) printf("decl -> VARIABLE : Atom, %s \n",(yyvsp[(1) - (3)].symp)->name);
             write_table(params,(yyvsp[(1) - (3)].symp)->name,_ATOM);
             params.flush();
           }
       ;}
    break;

  case 61:
#line 440 "parser.y"
    {
	      if(execute)
		{	 
		  if(ECHO_GRAMMAR) printf("extension : VARIABLE parameters   -- %s \n",(yyvsp[(1) - (3)].symp)->name);
		  list<struct Element>::iterator it = element_lookup((yyvsp[(1) - (3)].symp)->name);
		  list<struct Element>::iterator iterNULL = element_list.end();
		  if(it == iterNULL)
		    {
		      if(VERBOSE) printf("type %s has not been defined\n",(yyvsp[(1) - (3)].symp)->name);
		      (yyval.ival) = _NONE;
		    }
		  else
		    {
		      // inherit properties from the base type
		      (yyval.ival) = (*it).type;
		      inherit_properties(*it);
		    }
		  
		}
	    ;}
    break;

  case 62:
#line 463 "parser.y"
    {
	      if(execute)
		{	 
		  if(ECHO_GRAMMAR) printf("newinstance : VARIABLE -- %s \n",(yyvsp[(1) - (1)].symp)->name);
		  list<struct Element>::iterator it = element_lookup((yyvsp[(1) - (1)].symp)->name);
		  list<struct Element>::iterator iterNULL = element_list.end();
		  if(it == iterNULL)
		    {
		      if(VERBOSE) printf("type %s has not been defined\n",(yyvsp[(1) - (1)].symp)->name);
		      (yyval.ival) = _NONE;
		    }
		  else
		    {
		      // inherit properties from the base type
		      (yyval.ival) = (*it).type;
		      inherit_properties(*it);
		    }
		  
		}
	    ;}
    break;

  case 64:
#line 487 "parser.y"
    {
	      if(execute)
		{
#ifdef DEBUG 
                  printf("parameters, VARIABLE(%s) = aexpr(%.10g)\n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].dval));
#endif
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"l")) { params.l = (yyvsp[(3) - (5)].dval); params.lset = 1;} // length
		    else
	          if(!strcmp((yyvsp[(1) - (5)].symp)->name,"B")) { params.B = (yyvsp[(3) - (5)].dval); params.Bset = 1;} // dipole field
		    else 
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"ks")) { params.ks = (yyvsp[(3) - (5)].dval); params.ksset = 1;} // solenoid strength
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"k0")) { params.k0 = (yyvsp[(3) - (5)].dval); params.k0set = 1;} // dipole coef.
		    else 
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"k1")) { params.k1 = (yyvsp[(3) - (5)].dval); params.k1set = 1;} // quadrupole coef. 
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"k2")) { params.k2 = (yyvsp[(3) - (5)].dval); params.k2set = 1;} // sextupole coef.
		    else 
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"k3")) { params.k3 = (yyvsp[(3) - (5)].dval); params.k3set = 1;} // octupole coef.
		    else 
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"angle")) { params.angle = (yyvsp[(3) - (5)].dval); params.angleset = 1;} // dipole bending angle
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"phiAngleIn")) { params.phiAngleIn = (yyvsp[(3) - (5)].dval); params.phiAngleInset = 1;} // element incoming angle
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"phiAngleOut")) { params.phiAngleOut = (yyvsp[(3) - (5)].dval); params.phiAngleOutset = 1;} // element outgoing angle
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aper") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"aperture") ) 
		      { params.aper = (yyvsp[(3) - (5)].dval); params.aperset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aperX") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"apertureX") ) 
		      { params.aperX = (yyvsp[(3) - (5)].dval); params.aperXset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aperY") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"apertureY") ) 
		      { params.aperY = (yyvsp[(3) - (5)].dval); params.aperYset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aperYUp") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"apertureYUp") ) 
		      { params.aperYUp = (yyvsp[(3) - (5)].dval); params.aperYUpset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aperYDown") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"apertureYDown") ) 
		      { params.aperYDown = (yyvsp[(3) - (5)].dval); params.aperYDownset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"aperDy") ||!strcmp((yyvsp[(1) - (5)].symp)->name,"apertureDy") ) 
		    { params.aperDy = (yyvsp[(3) - (5)].dval); params.aperDyset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"outR") ) { params.outR = (yyvsp[(3) - (5)].dval); params.outRset = 1;}
		    else
                  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"inR") ) { params.inR = (yyvsp[(3) - (5)].dval); params.inRset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"xsize") ) { params.xsize = (yyvsp[(3) - (5)].dval); params.xsizeset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"ysize") ) { params.ysize = (yyvsp[(3) - (5)].dval); params.ysizeset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"tilt")) { params.tilt = (yyvsp[(3) - (5)].dval); params.tiltset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"x")) {params.xdir = (yyvsp[(3) - (5)].dval); params.xdirset = 1;} // x direction
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"y")) {params.ydir = (yyvsp[(3) - (5)].dval); params.ydirset = 1;} // y direction 
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"z")) {params.zdir = (yyvsp[(3) - (5)].dval); params.zdirset = 1;} // z direction 
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"phi")) {params.phi = (yyvsp[(3) - (5)].dval); params.phiset = 1;}  // polar angle
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"theta"))  {params.theta = (yyvsp[(3) - (5)].dval); params.thetaset = 1;} 
		  // azimuthal angle
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"psi"))  {params.psi = (yyvsp[(3) - (5)].dval); params.psiset = 1;} // 3rd  angle
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"gradient"))  {params.gradient = (yyvsp[(3) - (5)].dval); params.gradientset = 1;} // rf voltage
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"fint")) {;} // fringe field parameters
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"fintx")) {;}  //
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"tunnelRadius")) { params.tunnelRadius = (yyvsp[(3) - (5)].dval); params.tunnelRadiusset = 1;} // tunnel radius
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"tunnelOffsetX")) { params.tunnelOffsetX = (yyvsp[(3) - (5)].dval); params.tunnelOffsetXset = 1;} // tunnel offset
		  else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"precisionRegion")) { params.precisionRegion = (yyvsp[(3) - (5)].dval); params.precisionRegionset = 1;} // tunnel offset
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"e1")) {;}  //
                    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"e2")) {;}  //
                    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"hgap")) {params.hgap = (yyvsp[(3) - (5)].dval); params.hgapset=1;}  //
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"A")) {params.A = (yyvsp[(3) - (5)].dval); params.Aset = 1;}  // mass number
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"Z")) {params.Z = (yyvsp[(3) - (5)].dval); params.Zset = 1;}  // atomic number
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"density")) {params.density = (yyvsp[(3) - (5)].dval); params.densityset = 1;}  // density
                    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"T")) {params.temper = (yyvsp[(3) - (5)].dval); params.temperset = 1;}  // temperature
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"P")) {params.pressure = (yyvsp[(3) - (5)].dval); params.pressureset = 1;}  // pressure
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"waveLength")) {params.waveLength = (yyvsp[(3) - (5)].dval); params.waveLengthset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"taperlength")) {params.taperlength = (yyvsp[(3) - (5)].dval); params.taperlengthset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"flatlength")) {params.flatlength = (yyvsp[(3) - (5)].dval); params.flatlengthset = 1;}
                    else
		  if(!strcmp((yyvsp[(1) - (5)].symp)->name,"at")) {params.at = (yyvsp[(3) - (5)].dval); params.atset = 1;}  //position of an element within a sequence
		    else
                  if(VERBOSE) printf("Warning : unknown parameter %s\n",(yyvsp[(1) - (5)].symp)->name);
		  
		}
	    ;}
    break;

  case 65:
#line 595 "parser.y"
    {
	       if(execute) 
		 {
#ifdef DEBUG 
                   printf("params,VARIABLE (%s) = vecexpr (%d)\n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].array)->size);
#endif
                   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"knl")) 
		     {
		       params.knlset = 1;
		       set_vector(params.knl,(yyvsp[(3) - (5)].array));
		       delete[] (yyvsp[(3) - (5)].array)->data;
		     } 
		   else
		     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"ksl")) 
		       {
			 params.kslset = 1;
			 set_vector(params.ksl,(yyvsp[(3) - (5)].array));
			 delete[] (yyvsp[(3) - (5)].array)->data;
		       }
                     else
		     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"blmLocZ")) 
		       {
			 params.blmLocZset = 1;
			 set_vector(params.blmLocZ,(yyvsp[(3) - (5)].array));
			 delete[] (yyvsp[(3) - (5)].array)->data;
		       }
		   else
		     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"blmLocTheta")) 
		       {
			 params.blmLocThetaset = 1;
			 set_vector(params.blmLocTheta,(yyvsp[(3) - (5)].array));
			 delete[] (yyvsp[(3) - (5)].array)->data;
		       }
                   else
                     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"components"))
                       {
                         params.componentsset = 1;
                         set_vector(params.components,(yyvsp[(3) - (5)].array));
                         (yyvsp[(3) - (5)].array)->symbols.clear();
                       } 
                   else
                     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"componentsWeights"))
                       {
                         params.componentsWeightsset = 1;
                         set_vector(params.componentsWeights,(yyvsp[(3) - (5)].array));
                         delete[] (yyvsp[(3) - (5)].array)->data;
                       }
                   else
                     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"componentsFractions"))
                       {
                         params.componentsFractionsset = 1;
                         set_vector(params.componentsFractions,(yyvsp[(3) - (5)].array));
                         delete[] (yyvsp[(3) - (5)].array)->data;
                       }
		     else 	  
		       if(VERBOSE) printf("unknown parameter %s\n",(yyvsp[(1) - (5)].symp)->name);
		 }
	     ;}
    break;

  case 66:
#line 654 "parser.y"
    {
	       if(execute) 
		 {
#ifdef DEBUG 
                   printf("VARIABLE (%s) = vecexpr\n",(yyvsp[(1) - (3)].symp)->name);
#endif
		   if(!strcmp((yyvsp[(1) - (3)].symp)->name,"knl")) 
		     {
		       params.knlset = 1;
		       set_vector(params.knl,(yyvsp[(3) - (3)].array));
		       delete[] (yyvsp[(3) - (3)].array)->data;
		     } 
		   else
		     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"ksl")) 
		       {
			 params.kslset = 1;
			 set_vector(params.ksl,(yyvsp[(3) - (3)].array));
			 delete[] (yyvsp[(3) - (3)].array)->data;
		       }
                     else
		       if(!strcmp((yyvsp[(1) - (3)].symp)->name,"blmLocZ")) 
			 {
			   params.blmLocZset = 1;
			   set_vector(params.blmLocZ,(yyvsp[(3) - (3)].array));
			   delete[] (yyvsp[(3) - (3)].array)->data;
			 }
		       else
			 if(!strcmp((yyvsp[(1) - (3)].symp)->name,"blmLocTheta")) 
			   {
			     params.blmLocThetaset = 1;
			     set_vector(params.blmLocTheta,(yyvsp[(3) - (3)].array));
			     delete[] (yyvsp[(3) - (3)].array)->data;
			   }
			 else
                     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"components"))
                       {
                         params.componentsset = 1;
                         set_vector(params.components,(yyvsp[(3) - (3)].array));
                         delete[] (yyvsp[(3) - (3)].array)->data;
                       }
                   else
                     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"componentsWeights"))
                       {
                         params.componentsWeightsset = 1;
                         set_vector(params.componentsWeights,(yyvsp[(3) - (3)].array));
                         delete[] (yyvsp[(3) - (3)].array)->data;
                       }
                   else
                     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"componentsFractions"))
                       {
                         params.componentsFractionsset = 1;
                         set_vector(params.componentsFractions,(yyvsp[(3) - (3)].array));
                         delete[] (yyvsp[(3) - (3)].array)->data;
                       }
		   else 	  
		     if(VERBOSE) printf("unknown parameter %s\n",(yyvsp[(1) - (3)].symp)->name);
		 }         
	     ;}
    break;

  case 67:
#line 713 "parser.y"
    {
	      if(execute)
		{
#ifdef DEBUG 
                  printf("VARIABLE (%s) = aexpr(%.10g)\n",(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].dval));
#endif
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"l")) { params.l = (yyvsp[(3) - (3)].dval); params.lset = 1;} // length
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"B")) { params.B = (yyvsp[(3) - (3)].dval); params.Bset = 1;} // dipole field 
		    else 
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"ks")) { params.ks = (yyvsp[(3) - (3)].dval); params.ksset = 1;} // solenoid strength
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"k0")) { params.k0 = (yyvsp[(3) - (3)].dval); params.k0set = 1;} // dipole coef.
		    else 
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"k1")) { params.k1 = (yyvsp[(3) - (3)].dval); params.k1set = 1;} // quadrupole coef.
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"k2")) { params.k2 = (yyvsp[(3) - (3)].dval); params.k2set = 1;} // sextupole coef.
		    else 
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"k3")) { params.k3 = (yyvsp[(3) - (3)].dval); params.k3set = 1;} // octupole coef.
		    else 
		      if(!strcmp((yyvsp[(1) - (3)].symp)->name,"angle")) { params.angle = (yyvsp[(3) - (3)].dval); params.angleset = 1;} // dipole bending angle
		      else
			if(!strcmp((yyvsp[(1) - (3)].symp)->name,"phiAngleIn")) { params.phiAngleIn = (yyvsp[(3) - (3)].dval); params.phiAngleInset = 1;} // element incoming angle
			else
		      if(!strcmp((yyvsp[(1) - (3)].symp)->name,"phiAngleOut")) { params.phiAngleOut = (yyvsp[(3) - (3)].dval); params.phiAngleOutset = 1;} // element outgoing angle
		      else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"aper") ||!strcmp((yyvsp[(1) - (3)].symp)->name,"aperture") ) 
			      { params.aper = (yyvsp[(3) - (3)].dval); params.aperset = 1;}
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"aperX") ||!strcmp((yyvsp[(1) - (3)].symp)->name,"apertureX") ) 
			      { params.aperX = (yyvsp[(3) - (3)].dval); params.aperXset = 1;}
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"aperY") ||!strcmp((yyvsp[(1) - (3)].symp)->name,"apertureY") ) 
			      { params.aperY = (yyvsp[(3) - (3)].dval); params.aperYset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"outR") ) { params.outR = (yyvsp[(3) - (3)].dval); params.outRset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"xsize") ) { params.xsize = (yyvsp[(3) - (3)].dval); params.xsizeset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"ysize") ) { params.ysize = (yyvsp[(3) - (3)].dval); params.ysizeset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"tilt")) { params.tilt = (yyvsp[(3) - (3)].dval); params.tiltset = 1;}
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"x")) {params.xdir = (yyvsp[(3) - (3)].dval); params.xdirset = 1;} // x direction
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"y")) {params.ydir = (yyvsp[(3) - (3)].dval); params.ydirset = 1;} // y direction 
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"z")) {params.zdir = (yyvsp[(3) - (3)].dval); params.zdirset = 1;} // z direction 
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"phi")) {params.phi = (yyvsp[(3) - (3)].dval); params.phiset = 1;}  // polar angle
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"theta"))  {params.theta = (yyvsp[(3) - (3)].dval); params.thetaset = 1;} // azimuthal angle
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"psi"))  {params.psi = (yyvsp[(3) - (3)].dval); params.psiset = 1;} // 3rd angle
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"gradient"))  {params.gradient = (yyvsp[(3) - (3)].dval); params.gradientset = 1;} // rf voltage
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"fint")) {;} // fringe field parameters
		  else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"fintx")) {;}  //
		  else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"tunnelRadius")) { params.tunnelRadius = (yyvsp[(3) - (3)].dval); params.tunnelRadiusset = 1;} // tunnel radius
		  else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"tunnelOffsetX")) { params.tunnelOffsetX = (yyvsp[(3) - (3)].dval); params.tunnelOffsetXset = 1;} // tunnel offset
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"precisionRegion")) { params.precisionRegion = (yyvsp[(3) - (3)].dval); params.precisionRegionset = 1;} // tunnel offset
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"e1")) {;}  //
                    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"e2")) {;}  //
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"hgap")) {params.hgap = (yyvsp[(3) - (3)].dval); params.hgapset=1;}  //
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"A")) {params.A = (yyvsp[(3) - (3)].dval); params.Aset = 1;}  // mass number
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"Z")) {params.Z = (yyvsp[(3) - (3)].dval); params.Zset = 1;}  // atomic number
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"density")) {params.density = (yyvsp[(3) - (3)].dval); params.densityset = 1;}  // density
                    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"T")) {params.temper = (yyvsp[(3) - (3)].dval); params.temperset = 1;}  // temperature
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"P")) {params.pressure = (yyvsp[(3) - (3)].dval); params.pressureset = 1;}  // pressure
		  //else
		  //  if(!strcmp($1->name,"state")) {params.state = $3; params.stateset = 1;}  // state
		    else
		  if(!strcmp((yyvsp[(1) - (3)].symp)->name,"waveLength")) {params.waveLength = (yyvsp[(3) - (3)].dval); params.waveLengthset = 1;}
		    else
		  if(VERBOSE) printf("Warning : unknown parameter %s\n",(yyvsp[(1) - (3)].symp)->name);
		  
		}
	    ;}
    break;

  case 68:
#line 803 "parser.y"
    {
	       if(execute) 
		 {
#ifdef DEBUG 
                   printf("params,VARIABLE (%s) = str (%s)\n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].str));
#endif
		   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"geometry")) 
		     {
		       params.geomset = 1;
		       strcpy(params.geometry, (yyvsp[(3) - (5)].str));
		     } 
		   else
		     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"bmap")) 
		       {
			 params.geomset = 1;
			 strcpy(params.bmap, (yyvsp[(3) - (5)].str));
		       }
		   else 
		     if(!strcmp((yyvsp[(1) - (5)].symp)->name,"type")) 
		       {
			 //ignore the "type attribute for the moment"
		       }
		   else
		   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"material")) 
		       {
			 params.materialset = 1;
			 strcpy(params.material, (yyvsp[(3) - (5)].str));
		       }
		   else
		   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"tunnelMaterial")) 
		       {
			 params.tunnelmaterialset = 1;
			 strcpy(params.tunnelMaterial, (yyvsp[(3) - (5)].str));
		       }
		   else 
		   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"tunnelCavityMaterial")) 
		       {
			 params.tunnelcavitymaterialset = 1;
			 strcpy(params.tunnelCavityMaterial, (yyvsp[(3) - (5)].str));
		       }
		   else 
		   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"spec")) 
		       {
			 params.specset = 1;
			 strcpy(params.spec, (yyvsp[(3) - (5)].str));
		       }
                   else 
                   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"symbol"))
                       {
                         params.symbolset = 1;
                         strcpy(params.symbol, (yyvsp[(3) - (5)].str));
                       }
		   else 
                   if(!strcmp((yyvsp[(1) - (5)].symp)->name,"state"))
                       {
                         params.stateset = 1;
                         strcpy(params.state, (yyvsp[(3) - (5)].str));
                       }
		   else 
		   if(VERBOSE) printf("unknown parameter %s\n",(yyvsp[(1) - (5)].symp)->name);
		 }
	     ;}
    break;

  case 69:
#line 866 "parser.y"
    {
	       if(execute) 
		 {
#ifdef DEBUG 
                   printf("VARIABLE (%s) = str\n",(yyvsp[(1) - (3)].symp)->name);
#endif
		   if(!strcmp((yyvsp[(1) - (3)].symp)->name,"geometry")) 
		     {
		       params.geomset = 1;
		       strcpy(params.geometry, (yyvsp[(3) - (3)].str));
		     } 
		   else
		     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"bmap")) 
		       {
			 params.geomset = 1;
			 strcpy(params.bmap, (yyvsp[(3) - (3)].str));
		       }
		     else 
		     if(!strcmp((yyvsp[(1) - (3)].symp)->name,"type")) 
		       {
			 //ignore the "type attribute for the moment"
		       }
                     else
                       if(!strcmp((yyvsp[(1) - (3)].symp)->name,"material")) 
                         {	 
                           params.materialset = 1;
                           strcpy(params.material, (yyvsp[(3) - (3)].str));
                         }
                       else
                         if(!strcmp((yyvsp[(1) - (3)].symp)->name,"tunnelMaterial")) 
		       {	 
			 params.tunnelmaterialset = 1;
			 strcpy(params.tunnelMaterial, (yyvsp[(3) - (3)].str));
		       }
			 else
                         if(!strcmp((yyvsp[(1) - (3)].symp)->name,"tunnelCavityMaterial")) 
		       {	 
			 params.tunnelcavitymaterialset = 1;
			 strcpy(params.tunnelCavityMaterial, (yyvsp[(3) - (3)].str));
		       }
                     else
                   if(!strcmp((yyvsp[(1) - (3)].symp)->name,"spec")) 
		       {
			 params.specset = 1;
			 strcpy(params.spec, (yyvsp[(3) - (3)].str));
		       }
		   else 
                   if(!strcmp((yyvsp[(1) - (3)].symp)->name,"symbol"))
                       {
                         params.symbolset = 1;
                         strcpy(params.symbol, (yyvsp[(3) - (3)].str));
                       }
		   else 
                   if(!strcmp((yyvsp[(1) - (3)].symp)->name,"state"))
                       {
                         params.stateset = 1;
                         strcpy(params.state, (yyvsp[(3) - (3)].str));
                       }
		   else 
		   if(VERBOSE) printf("unknown parameter %s\n",(yyvsp[(1) - (3)].symp)->name);
		 }         
	     ;}
    break;

  case 74:
#line 947 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched sequence element, %s\n",(yyvsp[(1) - (3)].symp)->name);
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 75:
#line 964 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched sequence element, %s * %d \n",(yyvsp[(1) - (5)].symp)->name,(int)(yyvsp[(3) - (5)].dval));
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (5)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (5)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 76:
#line 982 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched sequence element, %s * %d \n",(yyvsp[(3) - (5)].symp)->name,(int)(yyvsp[(1) - (5)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (5)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (5)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 77:
#line 1000 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s\n",(yyvsp[(1) - (1)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (1)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 78:
#line 1017 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(1) - (3)].symp)->name,(int)(yyvsp[(3) - (3)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (3)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 79:
#line 1035 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(3) - (3)].symp)->name,(int)(yyvsp[(1) - (3)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (3)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (3)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 80:
#line 1053 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s\n",(yyvsp[(2) - (4)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(2) - (4)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
	    	      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 81:
#line 1070 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s\n",(yyvsp[(2) - (2)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(2) - (2)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
	    	      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 83:
#line 1090 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched sequence element, %s\n",(yyvsp[(1) - (3)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 84:
#line 1107 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched sequence element, %s * %d \n",(yyvsp[(1) - (5)].symp)->name,(int)(yyvsp[(3) - (5)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (5)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (5)].dval);i++)
			tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 85:
#line 1125 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched sequence element, %s * %d \n",(yyvsp[(3) - (5)].symp)->name,(int)(yyvsp[(1) - (5)].dval));
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (5)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (5)].dval);i++)
			tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 86:
#line 1143 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s\n",(yyvsp[(1) - (1)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (1)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 87:
#line 1160 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(1) - (3)].symp)->name,(int)(yyvsp[(3) - (3)].dval));
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (3)].dval);i++)
			tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 88:
#line 1178 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(3) - (3)].symp)->name,(int)(yyvsp[(1) - (3)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (3)].symp)->name;
		      e.type = _REV_LINE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (3)].dval);i++)
			tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 89:
#line 1196 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s\n",(yyvsp[(2) - (4)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(2) - (4)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
	    	      tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 90:
#line 1213 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s\n",(yyvsp[(2) - (2)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(2) - (2)].symp)->name;
		      e.type = _LINE;
		      e.lst = NULL;
	    	      tmp_list.push_back(e);
		    }
		  }
	      ;}
    break;

  case 92:
#line 1233 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched sequence element, %s\n",(yyvsp[(1) - (3)].symp)->name);
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 93:
#line 1250 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG 
                    printf("matched sequence element, %s * %d \n",(yyvsp[(1) - (5)].symp)->name,(int)(yyvsp[(3) - (5)].dval));
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (5)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (5)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 94:
#line 1268 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched sequence element, %s * %d \n",(yyvsp[(3) - (5)].symp)->name,(int)(yyvsp[(1) - (5)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (5)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (5)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 95:
#line 1286 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s\n",(yyvsp[(1) - (1)].symp)->name);
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (1)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 96:
#line 1303 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(1) - (3)].symp)->name,(int)(yyvsp[(3) - (3)].dval));
#endif
		    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(1) - (3)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(3) - (3)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 97:
#line 1321 "parser.y"
    {
		if(execute)
		  {
#ifdef DEBUG
                    printf("matched last sequence element, %s * %d\n",(yyvsp[(3) - (3)].symp)->name,(int)(yyvsp[(1) - (3)].dval));
#endif
                    // add to temporary element sequence
		    {
		      struct Element e;
		      e.name = (yyvsp[(3) - (3)].symp)->name;
		      e.type = _SEQUENCE;
		      e.lst = NULL;
		      for(int i=0;i<(int)(yyvsp[(1) - (3)].dval);i++)
			tmp_list.push_front(e);
		    }
		  }
	      ;}
    break;

  case 98:
#line 1341 "parser.y"
    { // check type ??
	 if(ECHO_GRAMMAR) printf("expr -> aexpr\n");
	 if(execute) 
	   {
	     if(INTERACTIVE) printf ("\t%.10g\n", (yyvsp[(1) - (1)].dval)); (yyval.dval)=(yyvsp[(1) - (1)].dval);
	   }
       ;}
    break;

  case 99:
#line 1349 "parser.y"
    {
	 if(ECHO_GRAMMAR) printf("expr -> vecexpr\n");
	 if(execute)
	   {
	     if(INTERACTIVE)
	       for(int i=0;i<(yyvsp[(1) - (1)].array)->size;i++)
		 {
		   printf(" %.10g ",(yyvsp[(1) - (1)].array)->data[i]);
		 }
	     (yyval.dval) = 0;
	   } 
       ;}
    break;

  case 100:
#line 1362 "parser.y"
    { // check type
	 if(ECHO_GRAMMAR) printf("expr -> assignment\n");
	 if(execute)
	   {
	     if(INTERACTIVE) {
	       if((yyvsp[(1) - (1)].symp)->type == _ARRAY)
		 {
		   for(list<double>::iterator it = (yyvsp[(1) - (1)].symp)->array.begin();
		       it!=(yyvsp[(1) - (1)].symp)->array.end();it++)
		     printf ("\t%.10g", (*it));
		   printf("\n");
		 }
	       else
		 printf ("\t%.10g\n", (yyvsp[(1) - (1)].symp)->value);
	     } 
	     (yyval.dval)=0;
	   }
       ;}
    break;

  case 101:
#line 1382 "parser.y"
    { (yyval.dval) = (yyvsp[(1) - (1)].dval);                         ;}
    break;

  case 102:
#line 1384 "parser.y"
    { 
	   //check type ??
	   (yyval.dval) = (yyvsp[(1) - (1)].symp)->value;        
          ;}
    break;

  case 103:
#line 1388 "parser.y"
    { (yyval.dval) = (*((yyvsp[(1) - (4)].symp)->funcptr))((yyvsp[(3) - (4)].dval));       ;}
    break;

  case 104:
#line 1389 "parser.y"
    { (yyval.dval) = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].dval);                    ;}
    break;

  case 105:
#line 1390 "parser.y"
    { (yyval.dval) = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].dval);                    ;}
    break;

  case 106:
#line 1391 "parser.y"
    { (yyval.dval) = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].dval);                    ;}
    break;

  case 107:
#line 1392 "parser.y"
    { (yyval.dval) = (yyvsp[(1) - (3)].dval) / (yyvsp[(3) - (3)].dval);                    ;}
    break;

  case 108:
#line 1393 "parser.y"
    { (yyval.dval) = pow((yyvsp[(1) - (3)].dval),(yyvsp[(3) - (3)].dval));                 ;}
    break;

  case 109:
#line 1394 "parser.y"
    { (yyval.dval) = -(yyvsp[(2) - (2)].dval); ;}
    break;

  case 110:
#line 1395 "parser.y"
    { (yyval.dval) = (yyvsp[(2) - (2)].dval); ;}
    break;

  case 111:
#line 1396 "parser.y"
    { (yyval.dval) = (yyvsp[(2) - (3)].dval);                         ;}
    break;

  case 112:
#line 1398 "parser.y"
    {
	   if((yyvsp[(2) - (5)].array)->size == (yyvsp[(4) - (5)].array)->size)
	     {
	       (yyval.dval) = 0;
	       for(int i=0;i<(yyvsp[(2) - (5)].array)->size;i++)
		 (yyval.dval) += (yyvsp[(2) - (5)].array)->data[i] * (yyvsp[(4) - (5)].array)->data[i];
	     }
	   else
	     {
	       if(VERBOSE) printf("vector dimensions do not match");
	       (yyval.dval) = _undefined;
	     }
         ;}
    break;

  case 113:
#line 1412 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) < (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 114:
#line 1413 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) <= (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 115:
#line 1414 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) > (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 116:
#line 1415 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) >= (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 117:
#line 1416 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) != (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 118:
#line 1417 "parser.y"
    { (yyval.dval) = ((yyvsp[(1) - (3)].dval) == (yyvsp[(3) - (3)].dval) )? 1 : 0; ;}
    break;

  case 119:
#line 1419 "parser.y"
    { 
	    if(ECHO_GRAMMAR) printf("aexpr-> %s [ %s ]\n ",(yyvsp[(1) - (4)].symp)->name, (yyvsp[(3) - (4)].symp)->name); 
	    (yyval.dval) = property_lookup((yyvsp[(1) - (4)].symp)->name,(yyvsp[(3) - (4)].symp)->name);
	  ;}
    break;

  case 120:
#line 1426 "parser.y"
    {
		if(ECHO_GRAMMAR) printf("%s \n",(yyvsp[(1) - (3)].symp)->name);
		if(execute)
		  {
		    if((yyvsp[(1) - (3)].symp)->is_reserved)
		      printf("%s is reserved",(yyvsp[(1) - (3)].symp)->name);
		    else
		      {
			(yyvsp[(1) - (3)].symp)->value = (yyvsp[(3) - (3)].dval); (yyval.symp)=(yyvsp[(1) - (3)].symp);       
		      }
		  }
	      ;}
    break;

  case 121:
#line 1439 "parser.y"
    {
		if(execute)
		  {
		    (yyvsp[(1) - (3)].symp)->array.erase((yyvsp[(1) - (3)].symp)->array.begin(),(yyvsp[(1) - (3)].symp)->array.end());
		    for(int i=0;i<(yyvsp[(3) - (3)].array)->size;i++)
		      (yyvsp[(1) - (3)].symp)->array.push_back((yyvsp[(3) - (3)].array)->data[i]);
		    (yyvsp[(1) - (3)].symp)->type = _ARRAY;
		    (yyval.symp) = (yyvsp[(1) - (3)].symp);
		    delete[] (yyvsp[(3) - (3)].array)->data;
		    (yyvsp[(3) - (3)].array)->size = 0;
		  }
              ;}
    break;

  case 122:
#line 1453 "parser.y"
    {
		if(execute)
		  {
		    (yyvsp[(1) - (3)].symp)->array.erase((yyvsp[(1) - (3)].symp)->array.begin(),(yyvsp[(1) - (3)].symp)->array.end());
		    for(int i=0;i<(yyvsp[(3) - (3)].array)->size;i++)
		      (yyvsp[(1) - (3)].symp)->array.push_back((yyvsp[(3) - (3)].array)->data[i]);
		    (yyval.symp) = (yyvsp[(1) - (3)].symp);
		    delete[] (yyvsp[(3) - (3)].array)->data;
		    (yyvsp[(3) - (3)].array)->size = 0;
		  }
              ;}
    break;

  case 123:
#line 1467 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->data = new double[(yyvsp[(1) - (1)].symp)->array.size()];
	      (yyval.array)->size = (yyvsp[(1) - (1)].symp)->array.size();
	      //array_list.push_back($$);
	      list<double>::iterator it;
	      int i = 0;
	      for(it=(yyvsp[(1) - (1)].symp)->array.begin();it!=(yyvsp[(1) - (1)].symp)->array.end();it++)
		{
		  (yyval.array)->data[i++] = (*it);
		}
	    }
        ;}
    break;

  case 124:
#line 1483 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->data = new double[(yyvsp[(1) - (1)].array)->size];
	      (yyval.array)->size = (yyvsp[(1) - (1)].array)->size;
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyvsp[(1) - (1)].array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (1)].array)->data[i];
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (1)].array)->data;
	      (yyvsp[(1) - (1)].array)->size = 0;
	    }
	;}
    break;

  case 125:
#line 1503 "parser.y"
    {
	  if(execute)
	  {
	    (yyval.array) = new struct Array;
	    (yyval.array)->size = (yyvsp[(1) - (1)].array)->size;
	    (yyval.array)->symbols = (yyvsp[(1) - (1)].array)->symbols;

	    (yyvsp[(1) - (1)].array)->symbols.clear();
	    (yyvsp[(1) - (1)].array)->size = 0;
	  }
	;}
    break;

  case 126:
#line 1516 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = ((yyvsp[(1) - (3)].array)->size < (yyvsp[(3) - (3)].array)->size )? (yyvsp[(1) - (3)].array)->size : (yyvsp[(3) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].array)->data[i] + (yyvsp[(3) - (3)].array)->data[i];
		}
	      
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (3)].array)->data;
	      delete[] (yyvsp[(3) - (3)].array)->data;
	      (yyvsp[(1) - (3)].array)->size = 0;
	      (yyvsp[(3) - (3)].array)->size = 0;
	    }
        ;}
    break;

  case 127:
#line 1539 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = ((yyvsp[(1) - (3)].array)->size < (yyvsp[(3) - (3)].array)->size )? (yyvsp[(1) - (3)].array)->size : (yyvsp[(3) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].array)->data[i] - (yyvsp[(3) - (3)].array)->data[i];
		}
	      
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (3)].array)->data;
	      delete[] (yyvsp[(3) - (3)].array)->data;
	      (yyvsp[(1) - (3)].array)->size = 0;
	      (yyvsp[(3) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 128:
#line 1562 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(1) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].array)->data[i] + (yyvsp[(3) - (3)].dval);
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (3)].array)->data;
	      (yyvsp[(1) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 129:
#line 1583 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(1) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].array)->data[i] * (yyvsp[(3) - (3)].dval);
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (3)].array)->data;
	      (yyvsp[(1) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 130:
#line 1603 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(1) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].array)->data[i] / (yyvsp[(3) - (3)].dval);
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(1) - (3)].array)->data;
	      (yyvsp[(1) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 131:
#line 1623 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(3) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(3) - (3)].array)->data[i] + (yyvsp[(1) - (3)].dval);
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(3) - (3)].array)->data;
	      (yyvsp[(3) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 132:
#line 1643 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(3) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].array)->data[i];
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(3) - (3)].array)->data;
	      (yyvsp[(3) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 133:
#line 1663 "parser.y"
    {
	  if(execute)
	    {
	      (yyval.array) = new struct Array;
	      (yyval.array)->size = (yyvsp[(3) - (3)].array)->size;
	      (yyval.array)->data = new double[(yyval.array)->size];
	      //array_list.push_back($$);
	      
	      for(int i=0;i<(yyval.array)->size;i++)
		{
		  (yyval.array)->data[i] = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].array)->data[i];
		}
	      
	      // erase data in vect
	      
	      delete[] (yyvsp[(3) - (3)].array)->data;
	      (yyvsp[(3) - (3)].array)->size = 0;
	    }
	;}
    break;

  case 134:
#line 1686 "parser.y"
    {
	    if(execute)
	      {
	        //printf("matched vector of size %d\n",_tmparray.size());
	        (yyval.array) = new struct Array;
	        (yyval.array)->data = new double[_tmparray.size()];
	        (yyval.array)->size = _tmparray.size();
      
	        //array_list.push_back(a);
      
	        list<double>::iterator it;
		int i=0;      
	        for(it=_tmparray.begin();it!=_tmparray.end();it++)
	  	{
	 	 (yyval.array)->data[i++] = (*it);
		}
    	        _tmparray.erase(_tmparray.begin(),_tmparray.end());

	        list<char*>::iterator lIter;
	        for(lIter = _tmpstring.begin(); lIter != _tmpstring.end(); lIter++)
	          (yyval.array)->symbols.push_back(*lIter);

	        _tmpstring.clear();
	      }
	;}
    break;

  case 135:
#line 1714 "parser.y"
    {
	  if(execute)
	  {
	    (yyval.array) = new struct Array;
	    (yyval.array)->size = _tmpstring.size();

	    list<char*>::iterator iter;
	    for(iter = _tmpstring.begin(); iter != _tmpstring.end(); iter++)
	      (yyval.array)->symbols.push_back(*iter);

	    _tmpstring.clear();
	  }
	;}
    break;

  case 137:
#line 1731 "parser.y"
    {
	    if(execute)
	      _tmparray.push_front((yyvsp[(1) - (3)].dval));
          ;}
    break;

  case 138:
#line 1736 "parser.y"
    {
	   if(execute)
	     _tmparray.push_front((yyvsp[(1) - (1)].dval));
        ;}
    break;

  case 140:
#line 1744 "parser.y"
    {
            if(execute)
              _tmpstring.push_front((yyvsp[(1) - (3)].str));
          ;}
    break;

  case 141:
#line 1749 "parser.y"
    {
           if(execute)
             _tmpstring.push_front((yyvsp[(1) - (1)].str));
         ;}
    break;

  case 142:
#line 1755 "parser.y"
    { if(execute) quit(); ;}
    break;

  case 144:
#line 1757 "parser.y"
    { if(execute) print( element_list ); ;}
    break;

  case 145:
#line 1758 "parser.y"
    { if(execute) print( beamline_list); ;}
    break;

  case 146:
#line 1759 "parser.y"
    { if(execute) print(options); ;}
    break;

  case 147:
#line 1761 "parser.y"
    {
	    if(execute)
	      {
		printf("\t");
		printf("\t%.10g\n",(yyvsp[(3) - (3)].symp)->value);
	      }
	  ;}
    break;

  case 148:
#line 1769 "parser.y"
    {
	    if(execute)
	      {
		printf("\t");
		
		list<double>::iterator it;
		for(it=(yyvsp[(3) - (3)].symp)->array.begin();it!=(yyvsp[(3) - (3)].symp)->array.end();it++)
		  {
		    printf("  %.10g ",(*it));
		  }
		
		printf("\n");
	      } 
	  ;}
    break;

  case 149:
#line 1783 "parser.y"
    { if(execute) expand_line(current_line,current_start, current_end);;}
    break;

  case 151:
#line 1785 "parser.y"
    { if(execute) printf("%s\n",(yyvsp[(2) - (2)].str)); ;}
    break;

  case 152:
#line 1787 "parser.y"
    {
	    if(execute)
	      {  
		if(ECHO_GRAMMAR) printf("command -> SAMPLE\n");
		add_sampler((yyvsp[(3) - (3)].symp)->name,(yyvsp[(3) - (3)].symp)->name, element_count);
		element_count = 1;
		params.flush();
	      }
          ;}
    break;

  case 153:
#line 1797 "parser.y"
    {
	    if(execute)
	      {  
		if(ECHO_GRAMMAR) printf("command -> CSAMPLE\n");
//SPM		add_csampler("sampler",$3->name, element_count,params.l, params.r);
		add_csampler((yyvsp[(3) - (3)].symp)->name,(yyvsp[(3) - (3)].symp)->name, element_count,params.l, params.r);
		element_count = 1;
		params.flush();
	      }
          ;}
    break;

  case 154:
#line 1808 "parser.y"
    {
	    if(execute)
	      {  
		if(ECHO_GRAMMAR) printf("command -> GAS\n");
		add_gas("gas",(yyvsp[(3) - (3)].symp)->name, element_count, params.material);
		element_count = 1;
		params.flush();
	      }
          ;}
    break;

  case 155:
#line 1818 "parser.y"
    {
	    if(execute)
	      {  
		if(ECHO_GRAMMAR) printf("command -> TUNNEL\n");
		write_table(params,"tunnel",_TUNNEL);
		params.flush();
	      }
          ;}
    break;

  case 156:
#line 1827 "parser.y"
    {
	    if(execute)
	      {  
		if(ECHO_GRAMMAR) printf("command -> BETA0\n");
	      }
          ;}
    break;

  case 157:
#line 1834 "parser.y"
    {
	    if(execute)
	      {
		set_value("doTwiss",1);
		if(ECHO_GRAMMAR) printf("command -> TWISS\n");
	      }
          ;}
    break;

  case 158:
#line 1842 "parser.y"
    {                                                   
            if(execute)                                       
              {                                               
                if(ECHO_GRAMMAR) printf("command -> DUMP\n"); 
                add_dump((yyvsp[(3) - (3)].symp)->name,(yyvsp[(3) - (3)].symp)->name, element_count);     
                element_count = 1;                            
                params.flush();                               
              }                                               
          ;}
    break;

  case 159:
#line 1856 "parser.y"
    {
		    if(execute)
		      {
			(yyval.str) = (yyvsp[(1) - (1)].symp)->name;
			current_line = (yyvsp[(1) - (1)].symp)->name;
			current_start = NULL;
			current_end = NULL;
		      }
                  ;}
    break;

  case 160:
#line 1866 "parser.y"
    {
		    if(execute)
		      {
			(yyval.str) = (yyvsp[(3) - (3)].symp)->name;
			current_line = (yyvsp[(3) - (3)].symp)->name;
			current_start = NULL;
			current_end = NULL;
		      }
                  ;}
    break;

  case 161:
#line 1876 "parser.y"
    {
		    if(execute)
		      {
			(yyval.str) = (yyvsp[(3) - (9)].symp)->name;
			current_line = (yyvsp[(3) - (9)].symp)->name;
			current_start = (yyvsp[(7) - (9)].symp)->name;
			current_end = (yyvsp[(9) - (9)].symp)->name;
		      }
		  ;}
    break;

  case 162:
#line 1890 "parser.y"
    { if(ECHO_GRAMMAR)  printf("sample_opt : RANGE =  %s \n",(yyvsp[(3) - (3)].symp)->name);
		  {
		    if(execute) (yyval.symp) = (yyvsp[(3) - (3)].symp);
		  }
                ;}
    break;

  case 163:
#line 1896 "parser.y"
    {
                  if(ECHO_GRAMMAR) printf("sample_opt : RANGE =  %s [%.10g] \n",(yyvsp[(3) - (6)].symp)->name,(yyvsp[(5) - (6)].dval));
		    {
		      if(execute) { (yyval.symp) = (yyvsp[(3) - (6)].symp); element_count = (int)(yyvsp[(5) - (6)].dval); }
		    }
                ;}
    break;

  case 164:
#line 1907 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt ->csopt , %s =  %.10g \n",(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].dval));
		    
		    if(execute)
		      {
			if( !strcmp((yyvsp[(1) - (3)].symp)->name,"r") ) params.r = (yyvsp[(3) - (3)].dval);
			else if (!strcmp((yyvsp[(1) - (3)].symp)->name,"l") ) params.l = (yyvsp[(3) - (3)].dval);
			else if(VERBOSE) 
			  printf("Warning : CSAMPLER: unknown parameter %s \n",(yyvsp[(1) - (3)].symp)->name);
		      }
		  ;}
    break;

  case 165:
#line 1919 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt -> %s =  %s \n",(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].str));
		    if(execute)
		      {
			;//set_value($1->name,string($3));
		      }
		  ;}
    break;

  case 166:
#line 1927 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt ->csopt , %s =  %.10g \n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].dval));
		    
		    if(execute)
		      {
			if( !strcmp((yyvsp[(1) - (5)].symp)->name,"r") ) params.r = (yyvsp[(3) - (5)].dval);
			else if (!strcmp((yyvsp[(1) - (5)].symp)->name,"l") ) params.l = (yyvsp[(3) - (5)].dval);
			else if(VERBOSE) 
			  printf("Warning : CSAMPLER: unknown parameter %s at line\n",(yyvsp[(1) - (5)].symp)->name);
		      }

		  ;}
    break;

  case 167:
#line 1940 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt -> %s =  %s \n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].str));
		    if(execute)
		      ;//set_value($1->name,string($3));
		  ;}
    break;

  case 168:
#line 1946 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt -> sopt, csopt\n");
		    (yyval.symp) = (yyvsp[(1) - (3)].symp);
		  ;}
    break;

  case 169:
#line 1951 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt -> sopt\n");
		    (yyval.symp) = (yyvsp[(1) - (1)].symp);
                  ;}
    break;

  case 170:
#line 1958 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> , %s =  %.10g \n",(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].dval));
		    
		    if(execute)
		      {
			if( !strcmp((yyvsp[(1) - (3)].symp)->name,"r") ) params.r = (yyvsp[(3) - (3)].dval);
			else if (!strcmp((yyvsp[(1) - (3)].symp)->name,"l") ) params.l = (yyvsp[(3) - (3)].dval);
			else if(VERBOSE) 
			  printf("Warning : GAS: unknown parameter %s \n",(yyvsp[(1) - (3)].symp)->name);
		      }
		  ;}
    break;

  case 171:
#line 1970 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> %s =  %s \n",(yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].str));
		    if(execute)
		      {
			if( !strcmp((yyvsp[(1) - (3)].symp)->name,"material") ) 
			  {
			    strcpy(params.material ,(yyvsp[(3) - (3)].str));
			    params.materialset = 1;
			  }
			//set_value($1->name,string($3));
		      }
		  ;}
    break;

  case 172:
#line 1983 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> , %s =  %.10g \n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].dval));
		    
		    if(execute)
		      {
			if( !strcmp((yyvsp[(1) - (5)].symp)->name,"r") ) params.r = (yyvsp[(3) - (5)].dval);
			else if (!strcmp((yyvsp[(1) - (5)].symp)->name,"l") ) params.l = (yyvsp[(3) - (5)].dval);
			else if(VERBOSE) 
			  printf("Warning : GAS: unknown parameter %s at line\n",(yyvsp[(1) - (5)].symp)->name);
		      }

		  ;}
    break;

  case 173:
#line 1996 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("csample_opt -> %s =  %s \n",(yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].str));
		    if(execute)
		      {
			  if( !strcmp((yyvsp[(1) - (5)].symp)->name,"material") ) 
			    {
			      strcpy(params.material ,(yyvsp[(3) - (5)].str));
			      params.materialset = 1;
			    }
		      }
		  ;}
    break;

  case 174:
#line 2008 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> range, csopt\n");

		  ;}
    break;

  case 175:
#line 2013 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> range\n");

                  ;}
    break;

  case 176:
#line 2018 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> range\n");
		    (yyval.symp) = (yyvsp[(3) - (5)].symp);
		  ;}
    break;

  case 177:
#line 2023 "parser.y"
    {
		    if(ECHO_GRAMMAR) printf("gas_opt -> range\n");
		    (yyval.symp) = (yyvsp[(3) - (3)].symp);
                  ;}
    break;

  case 179:
#line 2031 "parser.y"
    {
		      if(execute)
			set_value((yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].dval));
		    ;}
    break;

  case 180:
#line 2036 "parser.y"
    {
		      if(execute)
			set_value((yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].dval));
		    ;}
    break;

  case 181:
#line 2041 "parser.y"
    {
		      if(execute)
			set_value((yyvsp[(1) - (5)].symp)->name,std::string((yyvsp[(3) - (5)].str)));
		    ;}
    break;

  case 182:
#line 2046 "parser.y"
    {
		      if(execute)
			set_value((yyvsp[(1) - (3)].symp)->name,std::string((yyvsp[(3) - (3)].str)));
		    ;}
    break;

  case 184:
#line 2054 "parser.y"
    {
		    if(execute)
		      set_value((yyvsp[(1) - (5)].symp)->name,(yyvsp[(3) - (5)].dval));
		  ;}
    break;

  case 185:
#line 2059 "parser.y"
    {
		    if(execute)
		      set_value((yyvsp[(1) - (3)].symp)->name,(yyvsp[(3) - (3)].dval));
		  ;}
    break;

  case 186:
#line 2064 "parser.y"
    {
		    if(execute)
		      set_value((yyvsp[(1) - (5)].symp)->name,string((yyvsp[(3) - (5)].str)));
		  ;}
    break;

  case 187:
#line 2069 "parser.y"
    {
		    if(execute)
		      set_value((yyvsp[(1) - (3)].symp)->name,string((yyvsp[(3) - (3)].str)));
		  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 4188 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 2075 "parser.y"




int yyerror(char *s)
{
  printf("%s at line %d , file %s\n",s, line_num, yyfilename);
  exit(1);
}

#ifdef __cplusplus
extern "C" {
#endif
int yywrap()
{
	return 1;
}
#ifdef __cplusplus
}
#endif

