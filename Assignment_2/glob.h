#pragma once
#define SYM_TABLE_SIZE	100
#define HASH_TABLE_SIZE 11

extern yylex();
extern char* yytext;

int sym_table[SYM_TABLE_SIZE][2];
char str_pool[200];

typedef struct HTentry* HTpointer;
typedef struct HTentry {
    int index;
    HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];
