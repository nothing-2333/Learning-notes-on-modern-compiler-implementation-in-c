%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int charPos = 1;

int yywrap(void)
{
    charPos = 1;
    return 1;
}


void adjust(void)
{
    EM_tokPos = charPos;
    charPos += yyleng;
}

%}



%%
" "	        {adjust(); continue; }
\n	        {adjust(); EM_newline(); continue; }
","	        {adjust(); return COMMA;}

nil  	    {adjust(); return NIL;}
of  	    {adjust(); return OF;}
do  	    {adjust(); return DO;}
else  	    {adjust(); return ELSE;}
then  	    {adjust(); return THEN;}
if  	    {adjust(); return IF;}
array  	    {adjust(); return ARRAY;}
type  	    {adjust(); return TYPE;}
var  	    {adjust(); return VAR;}
function  	{adjust(); return FUNCTION;}
end  	    {adjust(); return END;}
in  	    {adjust(); return IN;}
let  	    {adjust(); return LET;}
break  	    {adjust(); return BREAK;}
to  	    {adjust(); return TO;}
while  	    {adjust(); return WHILE;}
for  	    {adjust(); return FOR;}

[0-9]+	                    {adjust(); yylval.ival = atoi(yytext); return INT;}
[a-zA-Z_][a-zA-Z_0-9]*      {adjust(); yylval.sval = yytext; return ID;}
/".*?/"

.	        {adjust(); EM_error(EM_tokPos, "illegal token");}
