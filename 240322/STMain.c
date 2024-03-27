#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "STMain.h"
#include "HashFunc.h"

char separators[] = " ,;\t\n\r\n";
char str_pool[100];
int sym_table[SYM_TABLE_SIZE][2];

typedef struct HTentry* HTpointer;
typedef struct HTentry {
    int index;
    HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];

HTpointer lookup_hash_table(int id_index, int hscode) {
    HTpointer entry = HT[hscode];

    while (entry != NULL) {
        if (strcmp(str_pool+sym_table[entry->index][0], str_pool+(id_index)) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
} 

void add_hash_table(int id_index, int hscode) {

    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    if (HT[hscode] == NULL) {
        HT[hscode] = newEntry;
    }
    else {
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}

void init_sym_table() {
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        sym_table[i][0] = -1;
        sym_table[i][1] = -1;
    }
}

void print_sym_table() {
    int i;
    printf("\nSymbol Table\n");
    printf("Index\tLength\tSymbol\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1) {
            printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool+ sym_table[i][0]);
        }
    }
}

int main() {
    FILE* fp;
    int result;
    int c; 
    int index_start = 0;
    int index_next = 0;
    int index = 0;
    int hash_value = -1;

    init_sym_table();

    result = fopen_s(&fp, "example.txt", "r"); 
    if (result != 0) {
        printf("Error - start with digit (%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF) { 
        if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1) {
        
            if (index_start < index_next) { 
                str_pool[index_next] = '\0'; 

                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9') { 
                    printf("Error - start with digit (%s)\n", str_pool + index_start); 
                    index_next = index_start; 
                }
                else {
                    int i = 0;
                    int hash_key = 0;
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);
                    
                    while(str_pool[index_start+i] != '\0'){
                        hash_key += str_pool[index_start + i++];
                    }
                    hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE); 

                    HTpointer htp = lookup_hash_table(index-1, hash_value);
                    
                    if (htp == NULL) {
                        add_hash_table(index-1, hash_value);
                        printf("%s (Hash: %d)\n", str_pool+sym_table[index-1][0], hash_value);
                        index_start = ++index_next;
                    }
                    else {
                        printf("%s (Already Exists. Hash: %d)\n", str_pool + sym_table[index-1][0], hash_value);
                        index_start = ++index_next;
                    }
                }
            }
            if (strchr(separators, c) == NULL) {
                str_pool[index_next++] = (char)c; 
            }
            continue; 
        }
        str_pool[index_next++] = (char)c; 
    }

    if (index_start < index_next) { 
        int i = 0;
        int hash_key = 0;
        str_pool[index_next] = '\0'; 
        sym_table[index][0] = index_start;
        sym_table[index++][1] = (int)strlen(str_pool + index_start);
        
        while(str_pool[index_start+i] != '\0'){
            hash_key += str_pool[index_start + i++];
        }
        hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE); 
        
        HTpointer htp = lookup_hash_table(index-1, hash_value);
                    
        if (htp == NULL) {
            add_hash_table(index-1, hash_value);
            printf("%s (Hash: %d)\n", str_pool+sym_table[index-1][0], hash_value);
        }
        else {
            printf("%s (Already Exists. Hash: %d)\n", str_pool + sym_table[index-1][0], hash_value);
        }
    }

    print_sym_table();

    fclose(fp); 
    return 0;
}
