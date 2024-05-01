#pragma once

#define SYM_TABLE_SIZE	100
#define HASH_TABLE_SIZE 11

#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z') || ((x) == '_')) 
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

char separators[] = " ,;\t\n\r\n";	//필요한지 생각하기
char str_pool[200];
int sym_table[SYM_TABLE_SIZE][4];
void init_sym_table();


typedef struct HTentry* HTpointer;
typedef struct HTentry {
    int index;
    HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];

int line_num = 1;

int index_start = 0;
int index_next = 0;
int hash_value = 0;
int sym_table_index = 0;
int flag = 0;