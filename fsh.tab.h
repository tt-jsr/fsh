/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
