#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

/*
	1. ���ڷ� �����ϴ� ���
		1) ���ڷθ� �̷���� ���
			ex) 123123
		2) ���ڵ� ���ԵǾ� �ִ� ���
			ex) 123id
	2. ������� �ʴ� ���ڸ� �����ϰ� �ִ� ���
		ex) l@ve
*/

void reporterror(int line_num, char* yytext, char* type) {
	printf("Error at line %d: %s is not a valid %s.\n", line_num, yytext, type);
}