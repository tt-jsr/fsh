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
[0-9]+\.[0-9]+ { yylval = new fsh::instruction::Float(strtod(yytext, nullptr)); return FSH_FLOAT; }
-?[0-9]+         { yylval = new fsh::instruction::Integer(atoi(yytext)); return FSH_INT; }
[a-zA-Z0-9]+   { yylval = new fsh::instruction::Identifier(yytext); return FSH_IDENTIFIER; }
%%

