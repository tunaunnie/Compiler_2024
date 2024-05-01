#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

int index_start = 0;
int index_next = 0;
int hash_value = 0;
int sym_table_index = 0;
int flag = 0;

int divisionMethod(char* key, int tableSize) {
    unsigned int hash_key = 0;
    int c;

    while (c = *key++) {
        hash_key += c;
    }
    return hash_key % tableSize;
}

HTpointer lookupHT(int id_index, int hscode) {
    HTpointer entry = HT[hscode];

    // 체이닝된 리스트를 탐색
    while (entry != NULL) {
        if (strcmp(str_pool + (entry->index), str_pool + (id_index)) == 0) {
            return entry; // 찾은 항목 반환
        }
        entry = entry->next;
    }
    return NULL; // 항목을 찾지 못한 경우
}

void addHT(int id_index, int hscode) {
    // 새 항목 생성 및 초기화
    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    if (HT[hscode] == NULL) {
        // 첫 번째 항목으로 추가
        HT[hscode] = newEntry;
    }
    else {
        // 이미 항목이 있으면, 리스트의 맨 앞에 추가
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}


void Symtable(int line_num, char* yytext, char* type) {
    int length = strlen(yytext);

    // Identifier의 길이가 15자를 초과하면 오류 처리
    if (length > 15) {
        printf("Error - Identifier length exceeds 15 characters.\n");
        index_next = index_start;
        flag = 1;
        return; // 오류 처리 후 함수 종료
    }

    // 식별자를 버퍼에 저장하기 전에 오버플로우 검사
    if (index_next + length + 1 > sizeof(str_pool)) {
        printf("Error - String Pool Overflow\n");
        flag = 1;
        return; // 오버플로우 처리 후 함수 종료
    }

    // 버퍼에 식별자를 저장
    strcpy(str_pool + index_start, yytext); // yytext를 str_pool에 저장
    index_next += length;
    str_pool[index_next++] = '\0';

    // 해시값 계산
    int hash_value = divisionMethod(str_pool+index_start, HASH_TABLE_SIZE);

    // 해시 테이블에서 식별자 검색
    HTpointer htp = lookupHT(index_start, hash_value);

    if (htp == NULL) {
        // 신규 식별자인 경우

        // 심볼 테이블에 식별자 정보 저장
        sym_table[sym_table_index][0] = index_start;
        sym_table[sym_table_index++][1] = length;

        addHT(index_start, hash_value);
        index_start = index_next; // 버퍼 인덱스 초기화

    }
    return;
    /*
    else {
        // 이미 존재하는 식별자인 경우
        printf("Error - Identifier '%s' already exists.\n", yytext);
        flag = 1;
    }
    */
}
