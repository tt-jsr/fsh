%{
#include <iostream>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>
#include "instrusive_ptr.h"
#include "common.h"
#include "element.h"
#include "bytecode.h"
#include "builtins.h"
#include "machine.h"
#include "ast.h"
#include "fsh.tab.h"
using namespace std;
#define YY_DECL extern int yylex()
uint64_t lineno = 1;
uint64_t column = 0;
bool commandMode = false;
bool interactive = false;
extern fsh::Machine machine;

extern "C" int yywrap() {
    return 1;
}

std::vector<YY_BUFFER_STATE> import_stack;

void ImportImpl(const char *pFile)
{
    interactive = false;
    import_stack.push_back(YY_CURRENT_BUFFER);
    yyin = fopen(pFile, "r");
    if (!yyin)
    {
        std::stringstream strm;
        strm << "Cannot open " << pFile;
        throw std::runtime_error(strm.str()); 
    }
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
}

%}

D           [0-9]
L           [a-zA-Z_]
E           [Ee][+-]?{D}+
H           [a-fA-F0-9]

%x          qstr
%x          cmd
%x          sys
%x          help
%x          import

%%
            std::string quotedstring;

[ \t]           {++column;}
[\n]            {++lineno;column = 0;}
^!.*            {system(yytext+1);}
\+              {++column;return '+';}
-               {++column;return '-';}
\.              {++column;return DOT;}
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
Return          {column += 6;return KW_RETURN;}
exit            {column += 4;exit(-1);}
global          {column += 6;return GLOBAL;}
import          {column += 6;BEGIN(import);}
<import>[ \t]*  /* eat white space */
<import>[^ \t\n]+ { /* Include file name*/
                    BEGIN(INITIAL);
                    ImportImpl(yytext);
                }
<<EOF>>         {
                    if (import_stack.size() == 0)
                    {
                        interactive = true;
                        yyterminate();
                    }
                    else
                    {
                        yy_delete_buffer(YY_CURRENT_BUFFER);
                        yy_switch_to_buffer(import_stack.back());
                        import_stack.pop_back();
                    }
                }
help            { BEGIN(help);}
<help>[ \t]*    /* eat whitespace */
<help>[a-zA-Z0-9_]+ {
                    std::string command;
                    command = "cat doc/";
                    if (*yytext == '\0')
                    {
                        command += "Intro.txt";
                    }
                    else
                    {
                        command += yytext;
                        command += ".txt";
                    }
                    system(command.c_str());
                    BEGIN(INITIAL);
                }
<help>\n        {
                    std::string command;
                    command = "cat doc/Intro.txt";
                    system(command.c_str());
                    BEGIN(INITIAL);
                }
\"              {quotedstring.clear(); BEGIN(qstr);}
<qstr>\"         {
                    BEGIN(INITIAL);
                    fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
                    bc->ctype = fsh::ASTConstant::CTYPE_STRING;
                    bc->svalue = quotedstring;
                    yylval = bc;
                    column += 1;
                    return STRING_LITERAL;
                }
<qstr>\n         { /* Unterminated string - error */
                    ++lineno;
                    column = 1;
                }

<qstr>\\n        {quotedstring.push_back('\n');++lineno;column=1;}
<qstr>\\t        quotedstring.push_back('\t');
<qstr>\\(.|\n)   quotedstring.push_back(yytext[1]);
<qstr>[^\\\n\"]+  {
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
                 yylval = new fsh::ASTSystem(lineno, yytext); 
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
    fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
    bc->ctype = fsh::ASTConstant::CTYPE_INTEGER;
    bc->ivalue = atoi(yytext);
    yylval = bc;
    return INTEGER; 
}

{D}+\.{D}+ { 
    column += strlen(yytext);
    fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
    bc->ctype = fsh::ASTConstant::CTYPE_FLOAT;
    bc->dvalue = strtod(yytext, nullptr);
    yylval = bc;
    return FLOAT; 
}

[a-zA-Z_][a-zA-Z0-9_]*   { 
    column += strlen(yytext);
    int64_t id = machine.string_table_add(yytext);
    fsh::ASTConstant *bc = new fsh::ASTConstant(lineno);
    bc->ctype = fsh::ASTConstant::CTYPE_IDENTIFIER;
    bc->svalue = yytext;
    yylval = bc;
    return IDENTIFIER; 
}
#.*$ ;
%%

void command_line()
{
    interactive = true;
	do {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        char *line;
        if (commandMode)
            line = readline("> ");
        else
            line = readline("[]: ");
        size_t len = strlen(line);
        if (len > 0)
            add_history(line);
        strncpy(buffer, line, sizeof(buffer));
        buffer[len] = '\n';
        ++len;
        buffer[sizeof(buffer)-1] = '\0';
        buffer[sizeof(buffer)-2] = '\0';
        //std::cout << buffer << std::endl;
        free(line);
        YY_BUFFER_STATE bs = yy_scan_bytes(buffer, len+2);
        //yy_switch_to_buffer(bs);
        //yy_flush_buffer(bs);
		yyparse();
        yy_delete_buffer(bs);
	} while (true);
}

