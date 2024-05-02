#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

/*
	1. 숫자로 시작하는 경우
			ex) 123id
	2. 허용하지 않는 문자를 포함하고 있는 경우
		ex) l@ve
*/

void reporterror1(int line_num, char* yytext) {

	printf("Error at line %d: start with digit.\n", line_num, yytext);
}

void reporterror2(int line_num, char* yytext) {

	printf("Error2 at line %d: %s is not valid.\n", line_num, yytext);
}

void reporterror3(int line_num, char* yytext) {

	printf("Error3 at line %d: %s is not valid.\n", line_num, yytext);
}
