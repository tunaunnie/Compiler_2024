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
"const"                     return(TCONST);
"else"                      return(TELSE);
"if"                        return(TIF);
"int"                       return(TINT);
"float"                     return(TFLOAT);
"return"                    return(TRETURN);
"void"                      return(TVOID);
"while"                     return(TWHILE);
"main"                      return(TMAIN);
"print"                     return(TPRINT);
"printf"                    return(TPRINT);
";"                         return(TSEMICOLON);
","                         return(TCOMMA);
"="                         return(TEQUAL);
"=="                        return(TEQUAL);
"!="                        return(TNOTEQU);
"<="                        return(TLESS);
">="                        return(TGREAT);
"&&"                        return(TAND);
"||"                        return(TOR);
"+"                         return(TPLUS);
"-"                         return(TMINUS);
"*"                         return(TSTAR);
"/"                         return(TMOD);
"++"                        return(TINC);
"--"                        return(TDEC);
"+="                        return(TADDASSIGN);
"-="                        return(TSUBASSIGN);
"*="                        return(TMULASSIGN);
"/="                        return(TDIVASSIGN);
"%="                        return(TMODASSIGN);

[A-Za-z_][A-Za-z0-9_]*              return(TIDENT); 
[0-9]*                              return(TNUMBER);
[0-9]+\.[0-9]+([eE][-+]?[0-9]+)?    return(TNUMBER);    // 소수점을 포함한 실수 형식 숫자
[0-9]+\.[0-9]+[fF]+                 return(TNUMBER);    // "f"를 포함한 고정소수점 형식 숫자 인식
[0-9]+([eE][-+]?[0-9]+)             return(TNUMBER);    // 지수 표기법 숫자
"("                                 return(TPAREN);     // 괄호 '('를 TPAREN 토큰으로 반환
")"                                 return(TPAREN);     // 괄호 ')'를 TPAREN 토큰으로 반환
"{"                                 return(TLBRACE);    // 중괄호 '{'를 TLBRACE 토큰으로 반환
"}"                                 return(TRBRACE);    // 중괄호 '}'를 TRBRACE 토큰으로 반환
"/*"([^*]|\*[^*/])*\**"*/"          ;
"//".*                              ;
[ \t]                                ;
"\n"                                return(LINEPROCESS);
[0-9]+[A-Za-z0-9_]*                 return(TERROR1);
[`~@#$^<>]*[A-Za-z0-9_]*[`~@#$^<>"]+[A-Za-z0-9_]*          return TERROR2; //두번째글자 이상에서 illegal있는 경우
.                                                       return TERROR3;
%%

int yywrap()
{
    return 1;
}
