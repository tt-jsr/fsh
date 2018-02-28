%{
#include <iostream>
#include "fsh.tab.h"
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
uint64_t lineno = 1;
%}
%%
[ \t] ;
[\n]        {++lineno;}
\+             {return '+';}
-             {return '-';}
\*             {return '*';}
\/             {return '/';}
=             {return '=';}
\(             {return '(';}
\)             {return ')';}
\{             {return '{';}
\}             {return '}';}
,               {return ',';}
\[               {return '[';}
\]               {return ']';}
&               {return '&';}
:               {return ':';}
\;               {return ';';}
\>               {return GT;}
\>=               {return GTE;}
\<               {return LT;}
\<=               {return LTE;}
==               {return EQ;}
\!=               {return NEQ;}
None            {return NONE;}
True            {return TRUE;}
False           {return FALSE;}
then            {return THEN;}
else            {return ELSE;}
If              {return IF;}
While           {return WHILE;}
\"[^\"\n]*\"    {
                    int len = strlen(yytext);
                    std::string s(yytext+1, len-2);
                    yylval = new fsh::instruction::String(s); 
                    return STRING;
                }
[0-9]+\.[0-9]+ { yylval = new fsh::instruction::Float(strtod(yytext, nullptr)); return FLOAT; }
[0-9]+         { yylval = new fsh::instruction::Integer(atoi(yytext)); return INT; }
[a-zA-Z0-9_]+   { yylval = new fsh::instruction::Identifier(yytext); return IDENTIFIER; }
\$[a-zA-Z0-9_]+   { yylval = new fsh::instruction::Identifier(yytext); return VARIABLE; }
#.*$ ;
%%

