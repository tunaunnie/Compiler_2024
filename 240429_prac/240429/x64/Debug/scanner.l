%{
/*
test.l
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tn.h"
#include "globe.h"
#include "main.c"
#include "symtable.c"


%}

%%
"const"                     return(TCONST);
"else"                      return(TELSE);
"if"                        return(TIF);
"int"                       return(TINT);
"return"                    return(TRETURN);
"void"                      return(TVOID);
"while"                     return(TWHILE);
"=="                        return(TEQUAL);
"!="                        return(TNOTEQU);
"<="                        return(TLESS);
">="                        return(TGREAT);
"&&"                        return(TAND);
"||"                        return(TOR);
"++"                        return(TINC);
"--"                        return(TDEC);
"+="                        return(TADDASSIGN);
"-="                        return(TSUBASSIGN);
"*="                        return(TMULASSIGN);
"/="                        return(TDIVASSIGN);
"%="                        return(TMODASSIGN);
"+"                         return(TPLUS);
"-"                         return(TMINUS);
"*"                         return(TSTAR);
"/"                         return(TSLASH);
"%"                         return(TMOD);
"="                         return(TASSIGN);
[A-Za-z_][A-Za-z0-9_]*      return(TIDENT);            //illeagal 여기서 처리 X. overflow 처리 O. 길이에러 처리 O. 
[1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?            return(TNUMBER);
"/*"([^*]|\*[^*/])*\**"*/"  ;
"//".*                      ;
[ \t]                       ;
"\n"                        return(LINEPROCESS);
.                           return(TERROR);            //illegal 처리 여기서 ?
%%

int yywrap()
{
    return 1;
}

