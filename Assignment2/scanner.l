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
"char"                      return(TCHAR);
"string"                    return(TSTRING);
"return"                    return(TRETURN);
"void"                      return(TVOID);
"while"                     return(TWHILE);
"main"                      return(TMAIN);
"print"                     return(TPRINT);
"printf"                    return(TPRINT);

"+"                         return(TPLUS);
"-"                         return(TMINUS);
"*"                         return(TMUL);
"/"                         return(TDIV);
"%"                         return(TMOD);

"="                         return(TASSIGN);
"+="                        return(TADDASSIGN);
"-="                        return(TSUBASSIGN);
"*="                        return(TMULASSIGN);
"/="                        return(TDIVASSIGN);
"%="                        return(TMODASSIGN);

"!"                         return(TNOT);
"&&"                        return(TAND);
"||"                        return(TOR);

"=="                        return(TEQUAL);
"!="                        return(TNOTEQU);
"<"                         return(TLESS);
">"                         return(TGREAT);
"<="                        return(TLESSE);
">="                        return(TGREATE);

"++"                        return(TINC);
"--"                        return(TDEC);

"("                         return(TLPAREN);     // 괄호 '('를 TPAREN 토큰으로 반환
")"                         return(TRPAREN);     // 괄호 ')'를 TPAREN 토큰으로 반환
"."                         return(TDOT);
","                         return(TCOMMA);
"{"                         return(TLBRACE);    // 중괄호 '{'를 TLBRACE 토큰으로 반환
"}"                         return(TRBRACE);    // 중괄호 '}'를 TRBRACE 토큰으로 반환
"["                         return(TLBRACKET);  // 대괄호 '['를 TLBRACKET 토큰으로 반환
"]"                         return(TRBRACKET);  // 대괄호 ']'를 TRBRACKET 토큰으로 반환
";"                         return(TSEMICOLON);
\"                          return(TQUOTE);


[A-Za-z_][A-Za-z0-9_]*                      return(TIDENT); 
[1-9][0-9]*|0([0-7]+|(X|x)[0-9A-Fa-f]+)?    return(TINTNUM);
[0-9]+\.[0-9]+([eE][-+]?[0-9]+)?            return(TFLOATNUM);    // 소수점을 포함한 실수 형식 숫자
[0-9]+\.[0-9]+[fF]+                         return(TFLOATNUM);    // "f"를 포함한 고정소수점 형식 숫자 인식
[0-9]+([eE][-+]?[0-9]+)                     return(TFLOATNUM);    // 지수 표기법 숫자

"/*"([^*]|\*[^*/])*\**"*/"          ;
"//".*                              ;
[ \t]                               ;
"\n"                                return(LINEPROCESS);

[0-9]+[A-Za-z0-9_]*                                return(TERROR1);
[`~@#$^]*[A-Za-z0-9_]*[`~@#$^]+[A-Za-z0-9_]*       return TERROR2;
.                                                  return TERROR3;
%%

int yywrap()
{
    return 1;
}