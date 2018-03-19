%{
#include <iostream>
#include "fsh.tab.h"
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern int yylex()
uint64_t lineno = 1;
uint64_t column = 0;
%}

D           [0-9]
L           [a-zA-Z_]
E           [Ee][+-]?{D}+
H           [a-fA-F0-9]

%x          str

%%
            std::string quotedstring;

[ \t]           {++column;}
[\n]            {++lineno;column = 0;}
\+              {++column;return '+';}
-               {++column;return '-';}
\*              {++column;return '*';}
\/              {++column;return '/';}
=               {++column;return '=';}
\(              {++column;return '(';}
\)              {++column;return ')';}
\{              {++column;return '{';}
\}              {++column;return '}';}
,               {++column;return ',';}
\[              {++column;return '[';}
\]              {++column;return ']';}
&               {++column;return '&';}
:               {++column;return ':';}
\.              {++column;return '.';}
\;              {++column;return ';';}
\>              {++column;return GT;}
\>=             {column += 2;return GTE;}
\<              {++column;return LT;}
\<=             {++column;return LTE;}
==              {++column;return EQ;}
->              {column += 2;return RIGHT_ARROW;}
\!=             {column += 2;return NEQ;}
\[\[            {column += 2;return DOUBLE_BRACKET_OPEN;}
\]\]            {column += 2;return DOUBLE_BRACKET_CLOSE;}
None            {column += 4;return NONE;}
True            {column += 4;return TRUE;}
False           {column += 5;return FALSE;}
then            {column += 4;return THEN;}
else            {column += 4;return ELSE;}
If              {column += 2;return IF;}
While           {column += 5;return WHILE;}
Try             {column += 5;return TRY;}
catch           {column += 5;return CATCH;}
and             {column += 3;return AND;}
or              {column += 2;return OR;}
in              {column += 2;return IN;}
For             {column += 3;return FOR;}
exit            {column += 4;exit(-1);}
\"              {quotedstring.clear(); BEGIN(str);}
<str>\"         {
                    BEGIN(INITIAL);
                    yylval = new fsh::instruction::String(lineno, quotedstring); 
                    column += 1;
                    return STRING_LITERAL;
                }
<str>\n         { /* Unterminated string - error */
                }

<str>\\n        quotedstring.push_back('\n');
<str>\\t        quotedstring.push_back('\t');
<str>\\(.|\n)   quotedstring.push_back(yytext[1]);
<str>[^\\\n\"]+  {
                    char *yptr = yytext;
                    while(*yptr)
                    {
                        quotedstring.push_back(*yptr++);
                        column += 1;
                    }
                }

^\/.*$           {column += 2;
                  yylval = strdup(yytext+1);
                  return COMMAND_LINE;
}

{D}+         { 
    column += strlen(yytext);
    yylval = new fsh::instruction::Integer(lineno, atoi(yytext)); 
    return INTEGER; 
}

{D}+\.{D}+ { 
    column += strlen(yytext);
    yylval = new fsh::instruction::Float(lineno, strtod(yytext, nullptr)); 
    return FLOAT; 
}

[a-zA-Z_][a-zA-Z0-9_]*   { 
    column += strlen(yytext);
    yylval = new fsh::instruction::Identifier(lineno, yytext); 
    return IDENTIFIER; 
}
#.*$ ;
%%

void command_line()
{
	do {
        yyin = stdin;
        YY_BUFFER_STATE bs = yy_create_buffer(yyin, YY_BUF_SIZE);
        yy_switch_to_buffer(bs);
        yy_flush_buffer(bs);
		yyparse();
        yy_delete_buffer(bs);
	} while (true);
}

