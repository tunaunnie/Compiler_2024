%{
/*
scanner.l
*/

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"
%}

%%

"const"                     { return TCONST; }
"else"                      { return TELSE; }
"if"                        { return TIF; }
"int"                       { return TINT; }
"return"                    { return TRETURN; }
"void"                      { return TVOID; }
"while"                     { return TWHILE; }
"=="                        { return TEQUAL; }
"!="                        { return TNOTEQU; }
"<="                        { return TLESS; }
">="                        { return TGREAT; }
"&&"                        { return TAND; }
"||"                        { return TOR; }
"+"                         { return TPLUS; }
"-"                         { return TMINUS; }
"*"                         { return TSTAR; }
"/"                         { return TSLASH; }
"%"                         { return TMOD; }
"++"                        { return TINC; }
"--"                        { return TDEC; }
"+="                        { return TADDASSIGN; }
"-="                        { return TSUBASSIGN; }
"*="                        { return TMULASSIGN; }
"/="                        { return TDIVASSIGN; }
"%="                        { return TMODASSIGN; }
[A-Za-z_][A-Za-z0-9_]*      { return TIDENT; }
[1-9][0-9]*|0([0-7]+|(x|X)[0-9A-Fa-f]*)? { return TNUMBER; }
"\/\*[^*]*\*+([^/*][^*]*\*+)*\/"       
"\/\/.*"                    ;
[ \t]+                      ;
"\n"                        { return LINEPROCESS; }
[0-9]+[A-Za-z0-9_]*         { return TERROR1; }     /* 숫자로 시작하는 경우 */
[A-Za-z0-9_]*[^A-Za-z0-9_ \t\n]+[A-Za-z0-9_]*  { return TERROR2; }  /* 허용되지 않은 문자 포함하는 경우 */

%%

int yywrap()
{
    return 1;
}
