%{
/*
test.l
*/

#include <stdio.h>
#include <stdlib.h>
enum tnumber { TEOF, TIDEN, TNUM, TASSIGN, TADD, TSEMI, TDOT, TBEGIN, TEND, TERROR};
%}

letter	[a-zA-Z_]
digit	[0-9]

%%
Mega-		yymore();
Kludge		ECHO;
Foobar		yyless(3); ECHO;
bar 			ECHO;
[a-z]+		printf("IDENT\n");
.			;
%%

void main()
{
	enum tnumber tn;  // token number
	while  ((tn = yylex()) != TEOF) {
	}
}

int yywrap()
{
	return 1;
}