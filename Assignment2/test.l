%{
/*
test.l
*/

#include <stdio.h>
#include <stdlib.h>
enum tnumber { TEOF, TIDEN, TNUM, TASSIGN, TADD, TSEMI, TDOT, TBEGIN, TEND, TERROR};
%}

%%
"int"       { printf("\nINT"); }
"{"         { printf("\nBEGIN"); }
"}"         { printf("\nEND"); }
[ \t\n]     ; // 공백 문자 무시
.           { printf("(%s)", yytext); }						
%%

int main()
{
    enum tnumber tn;  // token number
    while ((tn = yylex()) != TEOF) {
        // 토큰 처리 로직 추가
    }
    return 0;
}

int yywrap()
{
    return 1;
}
