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
[A-Za-z_][A-Za-z0-9_]*      return(TIDENT);            //illeagal 여기서 처리 X. overflow 처리 O. 길이에러 처리 O. 이미존재식별자 처리 O.
[1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)?            return(TNUMBER);
"/*"([^*]|\*[^*/])*\**"*/"  ;
"//".*                      ;
[ \t]                       ;
"\n"                        return(LINEPROCESS);
[A-Za-z0-9_]*[^A-Za-z0-9_ \t\n]+[A-Za-z0-9_]*         return(TERROR3);            //illegal 문자 에러 처리 O.
[0-9]+[A-Za-z0-9_]*         return(TERROR2);                  //숫자로 시작 에러 처리 O.           
%%

int yywrap()
{
    return 1;
}

