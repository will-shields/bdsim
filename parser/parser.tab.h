/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 191 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

