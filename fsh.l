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
\"[^\"\n]*\"    {
                    int len = strlen(yytext);
                    std::string s(yytext+1, len-2);
                    yylval = new fsh::instruction::String(s); 
                    column += len;
                    return STRING_LITERAL;
                }
[0-9]+\.[0-9]+ { column += strlen(yytext);yylval = new fsh::instruction::Float(strtod(yytext, nullptr)); return FLOAT; }
[0-9]+         { column += strlen(yytext);yylval = new fsh::instruction::Integer(atoi(yytext)); return INTEGER; }
[a-zA-Z0-9_]+   { column += strlen(yytext);yylval = new fsh::instruction::Identifier(yytext); return IDENTIFIER; }
#.*$ ;
%%

