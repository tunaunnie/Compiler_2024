/*
[ COMPILER ASSIGNMENT 01 _ CONTRIBUTION ]
1. 20 JAEUN JEON (33%)
2. 20 YEEUN LEE (33%)
3. 2176211 JIWON YANG (33%)

*/

/*
 [ COMPILER ASSIGNMENT 01 _ REQUIRED FUNCTIONS ]
 1. ERROR 1 - 식별자의 첫 글자에 숫자가 온 경우,
  : "Error - start with digit" 출력 후 다음 식별자로 넘어감.

 2. ERROR 2 - 식별자에 허용되지 않은 문자가 입력된 경우,
  : "Error - usuage of unable character" 출력 후 다음 식별자로 넘어감.

 3. ERROR 3 - 식별자가 최대 길이(15자)를 초과한 경우,
 : "Error - Inserted String is longer than 15 words" 출력 후 다음 식별자로 넘어감.

 4. ERROR 4 - 입력이 STRING POOL의 크기를 초과한 경우,
  : "Error - String Pool full." 출력 후,
  : STRING POOL을 초과하기 전 식별자까지 출력하고 읽기를 종료함.

 5. 동일한 식별자가 중복하여 입력된 경우,
  : "Already exists" 출력 후,
  : SYMBOL TABLE과 HASH TABLE에 새로 값을 저장하지 않되,
  : SYMBOL TABLE에서 기존에 입력된 식별자의 HASH 값을 찾아 출력함.

 6. 동일한 HASH 값을 가진 식별자가 입력된 경우 (충돌 발생),
 : 후방법으로 구현함.

  */

  /*
   [ COMPILER ASSIGNMENT 01 _ ADDITIONAL DEFINITIONS ]
   1. HASH BUCKET의 크기는 기호 상수 HASH_TABLE_SIZE,
      SYMBOL TABLE의 크기는 SYM_TABLE_SIZE를 이용해 정의함.
   2. HASH 값을 계산하기 위한 함수는 '제산법'을 사용함.
   3. 입력 파일의 마지막에 구분자가 없어도, 마지막 식별자까지 정상적으로 오류 처리가 가능하도록 구현함.
   4. SYMBOL TABLE을 순회하며 저장된 모든 식별자를 검색하는 함수 LOOKUP_SYM_TABLE을 추가로 구현함.

  */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "STMain.h"
#include "HashFunc.h"

char separators[] = " ,;\t\n\r\n"; // 구분자
char str_pool[100]; // string pool
int sym_table[SYM_TABLE_SIZE][2]; // symbol table 2차원 배열 생성

typedef struct HTentry* HTpointer;
typedef struct HTentry
{
    int index;
    HTpointer next;
} HTentry;

HTpointer HT[HASH_TABLE_SIZE];

// symbol table에서 식별자 검색 함수
bool lookup_sym_table(int cur_max_index, int cur_start_index)
{
    for (int i = 0; i < cur_max_index; i++) {

        // symbol table의 각 항목을 순회하며 주어진 식별자와 일치하는지 검사
        if (strcmp(str_pool + sym_table[i][0], str_pool + cur_start_index) == 0)
        {
            return true; // 식별자가 발견되면 true 반환
        }
    }
    return false; // 식별자가 발견되지 않으면 false 반환
}

// hash table에서 식별자 검색 함수
HTpointer lookup_hash_table(int id_index, int hscode)
{
    HTpointer entry = HT[hscode];

    while (entry != NULL)
    {
        // 해당 hash table 항목의 symbol table 인덱스를 사용하여 문자열을 비교하여 일치하는지 확인
        if (strcmp(str_pool + sym_table[entry->index][0], str_pool + id_index) == 0)
        {
            return entry; // 식별자가 발견되면 찾은 항목 반환
        }
        entry = entry->next; // 다음 항목으로 이동
    }
    return NULL; // 항목을 찾지 못한 경우
}

// hash table에 식별자 추가 함수
void add_hash_table(int id_index, int hscode)
{
    // 새 항목 생성 및 초기화
    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL)
    {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    // hash Table에 추가
    if (HT[hscode] == NULL)
    {
        HT[hscode] = newEntry;
    }
    else
    { // 만약 이미 존재한다면, 후위법으로 추가
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}

// symbol table 초기화 함수 (비어있는 table인지 아닌지 확인)
void init_sym_table()
{
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++)
    {
        sym_table[i][0] = -1;
        sym_table[i][1] = -1;
    }
}

// symbol table 출력 함수
void print_sym_table()
{
    int i;

    printf("\n    [[Symbol Table]]    \n");
    printf("--------------------------\n");
    printf("Index\tLength\tSymbol\n");

    for (i = 0; i < SYM_TABLE_SIZE; i++)
    {
        if (sym_table[i][0] != -1)
        {
            printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]);
        }
    }
}

// hash table 출력 함수
void print_hash_table()
{
    printf("\n    [[Hash Table]]    \n");
    printf("--------------------------\n");

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (HT[i] != NULL)
        {
            printf("Hash Code %d: ", i);
            HTpointer current = HT[i];

            while (current != NULL)
            {
                printf("%s -> ", str_pool + sym_table[current->index][0]);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main()
{
    FILE* fp;
    int result;
    int c; // 읽은 문자를 저장할 변수
    int index_start = 0; // 현재 들어온 식별자의 첫번째 위치
    int index_next = 0; // 다음에 들어올 식별자가 저장되어야하는 위치
    int index = 0;
    int hash_value = -1;

    init_sym_table();

    result = fopen_s(&fp, "example.txt", "r"); // "example.txt" 읽기 모드로 열기
    if (result != 0)
    {
        printf("(%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF)
    { // 파일 끝까지 문자 읽기

       /* string pool의 크기를 초과하는 경우 */
        if (index_next >= sizeof(str_pool) - 1)
        {
            printf("Error - String Pool full.\n"); // 에러 메시지 출력
            while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} // 이 문자열은 저장하지 않고 건너뜀
            index_next = index_start; // 인덱스 초기화
            fclose(fp); // 파일 닫기
            break;
        }
        else if (strchr(separators, c) != NULL) // 구분자를 만났을 때,
        {

            if (index_start < index_next)
            {
                str_pool[index_next] = '\0'; // 문자열 종료

                /* 식별자의 길이가 최대 길이 15자를 넘는 경우 */
                int str_len = index_next - index_start; // 들어온 문자열 길이 계산

                if (str_len >= MAX_STRING_LENGTH) // 15자보다 식별자 길이가 크다면,
                {
                    printf("Error - Inserted String is longer than 15 words\n"); // 에러 메시지 출력
                    for (int i = index_start; i < index_next; i++) { str_pool[i] = NULL; }
                    /*while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {}*/
                    index_next = index_start; // 인덱스 초기화
                    continue;
                }

                /* 식별자의 첫 글자가 숫자로 시작하는 경우 */
                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9')
                {
                    printf("Error - start with digit (%s)\n", str_pool + index_start); // 에러 메시지 출력
                    index_next = index_start; // 버퍼 인덱스 초기화
                }
                else
                {
                    // hash key 계산
                    int i = 0;
                    int hash_key = 0;
                    while (str_pool[index_start + i] != '\0')
                    {
                        hash_key += str_pool[index_start + i++]; // 주어진 문자열의 각 문자의 아스키 코드를 누적하여 hash key 생성
                    }
                    hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE); // 생성된 hash key를 hash table의 크기로 나누어 hash 값으로 변경

                    // 중복된 식별자 검사
                    bool isDuplicate = lookup_sym_table(index, index_start);
                    if (!isDuplicate)
                    {
                        // 중복된 식별자가 없으면, symbol table에 식별자 추가
                        sym_table[index][0] = index_start;
                        sym_table[index++][1] = (int)strlen(str_pool + index_start);



                        HTpointer htp = lookup_hash_table(index_start, hash_value); // 해당 hash 값으로 hash table을 조회하여 중복 확인

                        if (htp == NULL)
                        {
                            // 중복이 없으면, hash table에 추가
                            add_hash_table(index - 1, hash_value);
                            printf("%s (Hash: %d)\n", str_pool + sym_table[index - 1][0], hash_value); // 추가된 식별자와 해당하는 hash 값 출력
                            index_start = ++index_next; // 다음 식별자의 시작 인덱스를 설정

                        }
                    }
                    else
                    {
                        printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value);
                        index_next = index_start;
                    }
                }
            }

            // 구분자가 아닌 문자인 경우
            if (strchr(separators, c) == NULL)
            {
                str_pool[index_next++] = (char)c; // string pool에 저장
            }
            continue; // 다음 문자로 이동
        }

        /* 식별자에 영어 대소문자, 밑줄문자, 숫자, 구분자를 제외한 다른 문자가 입력될 경우 */
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_')))
        {
            printf("Error - usuage of unable character : %c\n", c); // 에러 메시지 출력
            while ((c = fgetc(fp)) != EOF && !(strchr(separators, c) != NULL)) {}
            index_next = index_start;
            continue;
        }

        else
            str_pool[index_next++] = (char)c;
    }

    if (index_start < index_next)
    { // 마지막 문자열 출력
        int i = 0;
        int hash_key = 0;
        int str_len = index_next - index_start; // 들어온 문자열 길이 계산

        // 중복된 식별자 검사
        bool isDuplicate = lookup_sym_table(index, index_start);

        // 중복된 식별자가 없는 경우
        if (!isDuplicate)
        {
            // 숫자로 시작하는지 검사
            // 숫자로 시작하지 않는 경우
            if (!(str_pool[index_start] >= '0' && str_pool[index_start] <= '9'))
            {
                // symbol table에 추가
                sym_table[index][0] = index_start;
                sym_table[index++][1] = (int)strlen(str_pool + index_start);

                // hash key 계산
                while (str_pool[index_start + i] != '\0')
                {
                    hash_key += str_pool[index_start + i++];
                }
                int hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE);

                // hash table에 추가
                HTpointer htp = lookup_hash_table(index_start, hash_value);
                if (htp == NULL)
                {
                    // 15자보다 식별자 길이가 크다면,
                    if (str_len >= MAX_STRING_LENGTH)
                    {
                        printf("Error - Inserted String is longer than 15 words\n"); // 에러 메시지 출력
                        while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {}
                    }
                    else
                    {
                        add_hash_table(index - 1, hash_value);
                        printf("%s (Hash: %d)\n", str_pool + index_start, hash_value); // 버퍼의 내용을 화면에 출력
                    }
                    index_next = ++index_start;
                }
            }
            else // 숫자로 시작하는 경우
            {
                printf("Error - start with digit (%s)\n", str_pool + index_start);  // 에러 메시지 출력
                index_next = index_start;
            }
        }
        //중복 식별자가 있는 경우
        else
        {
            printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value); // 에러 메시지 출력
            index_next = index_start;
        }
    }

    print_sym_table();
    print_hash_table();

    fclose(fp); //파일 닫기
    return 0;
}
