#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

/*
	1. 숫자로 시작하는 경우
		1) 숫자로만 이루어진 경우
			ex) 123123
		2) 문자도 포함되어 있는 경우
			ex) 123id
	2. 허용하지 않는 문자를 포함하고 있는 경우
		ex) l@ve
*/

void reporterror(int line_num, char* yytext, char* type) {
	printf("Error at line %d: %s is not a valid %s.\n", line_num, yytext, type);
}