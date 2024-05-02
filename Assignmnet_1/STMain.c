#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "STMain.h"

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
    printf("\n [[Symbol Table]] \n");
    printf("Index\tLength\tSymbol\n");
    printf("---------------------\n");
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
    int temp_start_index = 0;
    int temp_length = 0;
    HTPointer temp = NULL;

    printf("\n [[Hash Table]] \n");
    printf("---------------------\n");

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        temp = HT[i];
        if (temp == NULL)
            continue;
        else
        {
            printf("Hash Code  %d: %s", i, str_pool+(temp->index));
            temp = temp->next;
        }
        while (temp != NULL)
        {
            printf(" -> %s", str_pool+(temp->index));
            temp = temp->next;
        }
        printf("\n");
    }
}

// 잘 나오는데, 이 divisionMethod 왜 외부로부터 인클루드가 안 되지..?
int divisionMethod(char *key, int tableSize)
{
    unsigned int hash_key = 0;
    int c; 

    while (c = *key++)
    {                  
        hash_key += c; 
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
        printf("file open failure (%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF)
    {
        // 읽은 문자가 구분자인 경우. 한 단어가 끝남
        if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1)
        {
            if (index_start < index_next)
            {
                str_pool[index_next] = '\0';

                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9')
                {
                    printf("Error - start with digit (%s)\n", str_pool + index_start);
                    index_start = index_next;
                }
                else
                {
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

        // 읽은 문자가 허용되지 않은 문자인 경우. 중간에라도 들어오면 막아야 함.
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_')))
        {
            printf("Error - usuage of unable character : %c\n", c);
            while ((c = fgetc(fp)) != EOF && !(strchr(separators, c) != NULL)) { //구분자 나올 때까지 계속 읽음.
            } 
            index_next = index_start;
            continue;
        }
        // 읽은 문자가 구분자가 아닌 허용된 문자인 경우
        else
            str_pool[index_next++] = (char)c;
    }


    if (index_start < index_next)
    {
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

    fclose(fp);
    return 0;
}


/* ERROR ! */
/* example.text의 맨 마지막 문자가 식별자가 아니면 맨 마지막 에러를 못잡아냄 (if가 아니어서..) */