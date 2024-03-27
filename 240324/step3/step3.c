#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "step3.h"

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

// 잘 나오는데, 이 divisionMethod 왜 외부로부터 인클루드가 안 되지..?
int divisionMethod(char *key, int tableSize) {
    //여기서 key 인자는 해시값 구하고자 하는 하나의 단어 문자열 전체임.
    unsigned int hash_key = 0;
    int c;     //c는 정수형. 

    while (c = *key++) { //즉, 정수 c는 key 단어가 끝날때까지 (NULL일 때까지)
        hash_key += c;   //이 단어를 이루는 각 철자들의 아스키코드 값이 된다.
    }
    return hash_key % tableSize;
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
                else { 
                //심볼테이블에 저장하는 이 부분에서, 각 단어에 대한 해시값도 계산해주기
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);
                    
                    hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);
                    printf("%s (Hash: %d)\n", str_pool + index_start, hash_value); 
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
        //심볼테이블에 저장하는 이 부분에서, 각 단어에 대한 해시값도 계산해주기
        str_pool[index_next] = '\0'; 
        sym_table[index][0] = index_start;
        sym_table[index++][1] = (int)strlen(str_pool + index_start);

        /* 해시값 계산 함수에 바로 정수를 넘겨주고 싶으면 이렇게 해도 된다.
        int i = 0;
        int hash_key = 0;

        while(str_pool[index_start + i] != '\0'){
            hash_key += str_pool[index_start + i++];
        }
        hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE); */

        hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

        printf("%s (Hash: %d)\n", str_pool + index_start, hash_value); 
    }

    print_sym_table();

    fclose(fp); 
    return 0;
}