%{
#include <iostream>
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
%}
%%
[ \t\n] ;
'+'             {return '+';}
'-'             {return '-';}
'*'             {return '*';}
'/'             {return '/';}
'='             {return '=';}
[0-9]+\.[0-9]+ { yylval = instruction::Float(atod(yytext)); return FSH_FLOAT; }
[0-9]+         { yylval = instruction::Integer(atoi(yytext)); return FSH_INT; }
[a-zA-Z0-9]+   { yylval = instruction::Identifier(yytext); return FSH_IDENTIFIER; }
%%

