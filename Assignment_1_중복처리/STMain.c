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


// 해시 테이블에서 항목을 찾는 함수
HTpointer lookup_hash_table(int id_index, int hscode) {
    HTpointer entry = HT[hscode];

    while (entry != NULL) {
        if (strcmp(str_pool + sym_table[entry->index][0], str_pool + id_index) == 0) {
            return entry; // 찾은 항목 반환
        }
        entry = entry->next;
    }
    return NULL; // 항목을 찾지 못한 경우
}

void add_hash_table(int id_index, int hscode) {

    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    // Hash Table에 추가
    if (HT[hscode] == NULL) {
        HT[hscode] = newEntry;
    }
    else {
        // 후방법으로 추가
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

void print_sym_table() { //symbol table 출력하는 함수
    int i;
    printf("\nSymbol Table\n");
    printf("Index\tLength\tSymbol\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1) {
            printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]);
        }
    }
}

void print_hash_table() {
    printf("\n [[Hash Table]] \n");
    printf("---------------------\n");

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (HT[i] != NULL) {
            printf("Hash Code %d: ", i);
            HTpointer current = HT[i];

            while (current != NULL) {
                printf("%s -> ", str_pool + sym_table[current->index][0]);
                current = current->next;
            }
            printf("\n");
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

    result = fopen_s(&fp, "example.txt", "r"); // "example.txt" 파일 열기
    if (result != 0) {
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF) { // 파일 끝까지 문자 읽기

        if (index_next >= sizeof(str_pool) - 1) {
            printf("Error - String Pool 크기를 초과했습니다.\n");
            while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //이 문자열은 저장하지 않고 건너뜀.
            index_next = index_start; //인덱스 초기화
            //continue; //다음 문자로 이동해서 계속 진행함.
            fclose(fp); // 파일 닫기
            break; // 프로그램 종료
        }

        else if (strchr(separators, c) != NULL) {
            // 구분자를 만나거나 버퍼 크기 제한에 도달했을 때
            if (index_start < index_next) { // 버퍼에 내용이 있을 경우만 출력
                str_pool[index_next] = '\0'; // 문자열 종료

                /* 15자 안 넘게 추가한 부분*/
                int str_len = index_next - index_start; //들어온 문자열 길이 계산
                if (str_len > MAX_STRING_LENGTH) { //15자 보다 식별자 길이가 크다면
                    printf("Error - Inserted String is longer than 15 words");

                    while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //이 문자열은 저장하지 않고 건너뜀.
                    index_next = index_start; //인덱스 초기화
                    continue; //다음 문자로 이동해서 계속 진행함.
                }

                str_pool[index_next] = '\0';

                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9') { // 문자열이 숫자로 시작할 경우
                    printf("Error - start with digit (%s)\n", str_pool + index_start); // 에러 출력
                    index_next = index_start; // 버퍼 인덱스 초기화
                }
                else {
                    int i = 0;
                    int hash_key = 0;
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);

                    while (str_pool[index_start + i] != '\0') { //마지막이 NULL이 아닌 경우
                        hash_key += str_pool[index_start + i++];
                    }
                    hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE);

                    HTpointer htp = lookup_hash_table(index_start, hash_value);
                    if (htp == NULL) {
                        add_hash_table(index - 1, hash_value);
                        printf("%s (Hash: %d)\n", str_pool + sym_table[index - 1][0], hash_value); // 버퍼의 내용을 화면에 출력
                        index_start = ++index_next; // 버퍼 인덱스 초기화
                    }
                    else {
                        printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value); // 버퍼의 내용을 화면에 출력
                        index_start = ++index_next; // 버퍼 인덱스 초기화
                    }
                }
            }
            if (strchr(separators, c) == NULL) {
                str_pool[index_next++] = (char)c; // 구분자가 아닌 문자를 버퍼에 저장
            }
            continue; // 다음 문자로 이동
        }
        // 읽은 문자가 허용되지 않은 문자인 경우. 중간에라도 들어오면 막아야 함.
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_')))
        {
            printf("Error - usuage of unable character : %c\n", c);
            while ((c = fgetc(fp)) != EOF && !(strchr(separators, c) != NULL)) {
            }
            index_next = index_start;
            continue;
        }
        // 읽은 문자가 구분자가 아닌 허용된 문자인 경우
        else
            str_pool[index_next++] = (char)c;
    }

    if (index_start < index_next) { // 마지막 문자열 출력
        int i = 0;
        int hash_key = 0;

        str_pool[index_next] = '\0'; // 문자열 종료
        sym_table[index][0] = index_start;
        sym_table[index++][1] = (int)strlen(str_pool + index_start);

        while (str_pool[index_start + i] != '\0') {
            hash_key += str_pool[index_start + i++];
        }
        hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE);


        HTpointer htp = lookup_hash_table(index_start, hash_value);
        if (htp == NULL) {
            add_hash_table(index_start, hash_value);
            printf("%s (Hash: %d)\n", str_pool + index_start, hash_value); // 버퍼의 내용을 화면에 출력
        }
        else {
            printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value); // 버퍼의 내용을 화면에 출력
        }

    }

    print_sym_table();
    print_hash_table();

    fclose(fp); // 파일 닫기
    return 0;
}