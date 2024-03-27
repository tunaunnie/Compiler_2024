#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "step2.h"

char separators[] = " ,;\t\n\r\n";
char str_pool[100];
int sym_table[SYM_TABLE_SIZE][2];

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

    init_sym_table();

    result = fopen_s(&fp, "example.txt", "r"); 
    if (result != 0) {
        printf("���� ���� ����(%d)\n", result);
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
                else { //원래 한 단어씩 출력하던 여기에 코드 추가됨.
                //식별자 들어와서, 한 단어 단위가 이제 끊겼을 때,
                //심벌테이블에 시작인덱스 저장하고, 길이 저장함.
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);
                    printf("%s\n", str_pool + index_start); 
                    index_start = ++index_next; 
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
        str_pool[index_next] = '\0'; 
        sym_table[index][0] = index_start;
        sym_table[index++][1] = (int)strlen(str_pool + index_start);
        printf("%s\n", str_pool + index_start); 
    }

    print_sym_table();

    fclose(fp); 
    return 0;
}
