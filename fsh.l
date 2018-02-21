%{
#include <iostream>
#include "fsh.tab.h"
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
%}
%%
[ \t\n] ;
\+             {return '+';}
-             {return '-';}
\*             {return '*';}
\/             {return '/';}
=             {return '=';}
\(             {return '(';}
\)             {return ')';}
,               {return ',';}
\[               {return '[';}
\]               {return ']';}
[0-9]+\.[0-9]+ { yylval = new fsh::instruction::Float(strtod(yytext, nullptr)); return FLOAT; }
[0-9]+         { yylval = new fsh::instruction::Integer(atoi(yytext)); return INT; }
\$?[a-zA-Z0-9_]+   { yylval = new fsh::instruction::Identifier(yytext); return IDENTIFIER; }
%%

