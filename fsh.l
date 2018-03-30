%{
#include <iostream>
#include "fsh.tab.h"
#include "common.h"
#include "instructions.h"
using namespace std;
#define YY_DECL extern int yylex()
uint64_t lineno = 1;
uint64_t column = 0;
bool commandMode = false;
bool interactive = false;

extern "C" int yywrap() {
    return 1;
}
%}

D           [0-9]
L           [a-zA-Z_]
E           [Ee][+-]?{D}+
H           [a-fA-F0-9]

%x          str
%x          cmd
%x          sys

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
                    ++lineno;
                    column = 1;
                }

<str>\\n        {quotedstring.push_back('\n');++lineno;column=1;}
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
^\<sh\>$              {column += 4;
                 BEGIN(sys);
                }
<sys>^\<\/sh\>$  {column+=5;
                 BEGIN(INITIAL);
                 return ';';
                }
<sys>^[^\<]+$    {column += strlen(yytext);
                 yylval = new fsh::instruction::System(lineno, yytext); 
                 return SYSTEM; 
                }
<sys>\n       {
                 column = 0;
                 ++lineno;
                }
                 

^\/$            {column += 1;
                 BEGIN(cmd);
                 commandMode = true;
                 yyterminate();
                }
<cmd>^\/        {column += 1;
                 BEGIN(INITIAL);
                 commandMode= false;
                 yyterminate();
                }
<cmd>[^|<>\n&; ]+    {column += strlen(yytext);
                 yylval = strdup(yytext);
                 return CMD_WORD;
                }
<cmd>\&         {column += 1;
                 return CMD_AMP;
                }
<cmd>\;         {column += 1;
                 return CMD_SEMI;
                }
<cmd>\|         {column += 1;
                 return CMD_BAR;
                }
<cmd>\<         {column += 1;
                 return CMD_LT;
                }
<cmd>\>         {column += 1;
                 return CMD_GT;
                }
<cmd>\>\>       {column += 1;
                 return CMD_GTGT;
                }
<cmd>\n         {lineno += 1;
                 column = 0;
                 return CMD_NL;
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
    interactive = true;
	do {
        if (commandMode)
            std::cout << "> ";
        else
            std::cout << "[]: ";
        yyin = stdin;
        YY_BUFFER_STATE bs = yy_create_buffer(yyin, YY_BUF_SIZE);
        yy_switch_to_buffer(bs);
        yy_flush_buffer(bs);
		yyparse();
        yy_delete_buffer(bs);
	} while (true);
}

