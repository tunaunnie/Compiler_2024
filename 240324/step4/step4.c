#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "step4.h"

char separators[] = " ,;\t\n\r\n";
char str_pool[100];
int sym_table[SYM_TABLE_SIZE][2];

typedef struct HTentry *HTPointer;
typedef struct HTentry
{
    int index;
    HTPointer next;
} HTentry;

HTPointer HT[HASH_TABLE_SIZE];

HTPointer lookup_hash_table(int id_index, int hscode)
{
    HTPointer entry = HT[hscode];

    while (entry != NULL)
    {
        if (strcmp(str_pool + (entry->index), str_pool + (id_index)) == 0)
        {
            return entry; // 찾은 항목을 반환
        }
        entry = entry->next;
    }
    return NULL;
}

void add_hash_table(int id_index, int hscode)
{
    // 새 항목 생성 및 초기화
    HTPointer newEntry = (HTPointer)malloc(sizeof(HTentry));
    if (newEntry == NULL)
    {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    /*HT에 추가*/
    // 1. 해당 해시값을 가진 엔트리가 없었다면, 그냥 추가해주면 됨
    if (HT[hscode] == NULL)
    {
        HT[hscode] = newEntry;
    }
    else
    { // 2. 그 해시값을 가진 엔트리가 이미 있었다면(충돌 발생) 충돌 처리 (후위법)
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}

void init_sym_table()
{
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++)
    {
        sym_table[i][0] = -1;
        sym_table[i][1] = -1;
    }
}

void print_sym_table()
{
    int i;
    printf("\nSymbol Table\n");
    printf("Index\tLength\tSymbol\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++)
    {
        if (sym_table[i][0] != -1)
        {
            printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]);
        }
    }
}

void print_ht_table()
{
    HTPointer temp = NULL;
    printf("\nHash Table\n");

    for (int i = 0; i<HASH_TABLE_SIZE; i++){
        temp = HT[i];
        if (temp == NULL)
            printf("[%d]: NULL", i);
        else {
            printf("[%d]: %d ", i, temp->index);
            temp = temp->next; 
        }
        while (temp != NULL) {
            printf("-> %d",temp->index);
            temp = temp->next;
        }
        printf("\n");
    }

}

// 잘 나오는데, 이 divisionMethod 왜 외부로부터 인클루드가 안 되지..?
int divisionMethod(char *key, int tableSize)
{
    // 여기서 key 인자는 해시값 구하고자 하는 하나의 단어 문자열 전체임.
    unsigned int hash_key = 0;
    int c; // c는 정수형.

    while (c = *key++)
    {                  // 즉, 정수 c는 key 단어가 끝날때까지 (NULL일 때까지)
        hash_key += c; // 이 단어를 이루는 각 철자들의 아스키코드 값이 된다.
    }
    return hash_key % tableSize;
}

int main()
{
    FILE *fp;
    int result;
    int c;
    int index_start = 0;
    int index_next = 0;
    int index = 0;
    int hash_value = -1;

    init_sym_table();

    result = fopen_s(&fp, "example.txt", "r");
    if (result != 0)
    {
        printf("���� ���� ����(%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF)
    {
        if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1)
        {

            if (index_start < index_next)
            {
                str_pool[index_next] = '\0';

                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9')
                {
                    printf("Error - start with digit (%s)\n", str_pool + index_start);
                    index_next = index_start;
                }
                else
                {
                    // 심볼테이블에 저장하는 이 부분에서, 각 단어에 대한 해시값도 계산해주기
                    // 해시값 계산한 후, HT의 해시값 인덱스에다가 지금 ST에서 시작하는 인덱스 저장해주기
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);
                    hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

                    HTPointer htp = lookup_hash_table(index_start, hash_value);
                    if (htp == NULL)
                    {
                        add_hash_table(index_start, hash_value);
                        printf("%s (Hash: %d)\n", str_pool + index_start, hash_value);
                        index_start = ++index_next;
                    }
                    else
                    {
                        printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value);
                        index_start = ++index_next;
                    }
                }
            }
            if (strchr(separators, c) == NULL)
            {
                str_pool[index_next++] = (char)c;
            }
            continue;
        }
        str_pool[index_next++] = (char)c;
    }

    if (index_start < index_next)
    {
        // 심볼테이블에 저장하는 이 부분에서, 각 단어에 대한 해시값도 계산해주기
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

        HTPointer htp = lookup_hash_table(index_start, hash_value);
        if (htp == NULL)
        {
            add_hash_table(index_start, hash_value);
            printf("%s (Hash: %d)\n", str_pool + index_start, hash_value);
        }
        else
        {
            printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value);
        }
    }

    print_sym_table();
    print_ht_table();

    /*HT 값 출력하는 거까지 완성해보라는데..*/

    fclose(fp);
    return 0;
}

