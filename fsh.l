%{
#include <iostream>
#include "fsh.tab.h"
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
uint64_t lineno = 1;
uint64_t column = 0;
%}

D           [0-9]
L           [a-zA-Z_]
E           [Ee][+-]?{D}+
H           [a-fA-F0-9]

%%
[ \t]       {++column;}
[\n]        {++lineno;column = 0;}
\+             {++column;return '+';}
-             {++column;return '-';}
\*             {++column;return '*';}
\/             {++column;return '/';}
=             {++column;return '=';}
\(             {++column;return '(';}
\)             {++column;return ')';}
\{             {++column;return '{';}
\}             {++column;return '}';}
,               {++column;return ',';}
\[               {++column;return '[';}
\]               {++column;return ']';}
&               {++column;return '&';}
:               {++column;return ':';}
\.               {++column;return '.';}
\;               {++column;return ';';}
\>               {++column;return GT;}
\>=               {column += 2;return GTE;}
\<               {++column;return LT;}
\<=               {++column;return LTE;}
==               {++column;return EQ;}
\!=               {column += 2;return NEQ;}
None            {column += 4;return NONE;}
True            {column += 4;return TRUE;}
False           {column += 5;return FALSE;}
then            {column += 4;return THEN;}
else            {column += 4;return ELSE;}
If              {column += 2;return IF;}
While           {column += 5;return WHILE;}
Try             {column += 5;return TRY;}
catch           {column += 5;return CATCH;}
Part            {column += 4;return PART;}
and             {column += 3;return AND;}
or              {column += 2;return OR;}
\"[^\"\n]*\"    {
                    int len = strlen(yytext);
                    std::string s(yytext+1, len-2);
                    yylval = new fsh::instruction::String(s); 
                    column += len;
                    return STRING_LITERAL;
                }
{D}+         { column += strlen(yytext);yylval = new fsh::instruction::Integer(atoi(yytext)); return INTEGER; }
{D}+\.{D}+ { column += strlen(yytext);yylval = new fsh::instruction::Float(strtod(yytext, nullptr)); return FLOAT; }
[a-zA-Z_][a-zA-Z0-9_]*   { column += strlen(yytext);yylval = new fsh::instruction::Identifier(yytext); return IDENTIFIER; }
#.*$ ;
%%

