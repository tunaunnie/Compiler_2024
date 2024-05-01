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
    // 저장해야 할 Identifier가 yytext 형태로 저장되어 들어옴.
    // 숫자시작, illegal ident 에러 들어오는 경우 없음.
    // 15자 이상 처리, 오버플로우 처리만 하면 됨
    // '마지막 문자열 출력' 따로 뺄 필요가 없는건가?

    int length = strlen(yytext);            //입력이 number인 경우 에러뜸 (15자 이상인 걸로 뜬다 ..)

    //오버플로우 처리. 넘어가면 return해서 끝나도록
    //한 글자씩 넣는 게 아니고 yytext 단어를 한꺼번에 넣는 것. 한번에 들어오는 길이로 비교
    if (index_next + length > sizeof(str_pool)) {     //-1 안해도 되겠지..?

        printf("String Pool Overflow\n");
        printST();
        printHT();
        flag = 1;

        return 1;
    }

    //15자 이상 들어오면 처리
    if (length > 15) {
        printf("Error - Exceed\n");
        index_next = index_start;
        flag = 1;

        return 1;
    }

    //15자 이상도 아니고 오버플로우도 안 나면 버퍼에 단어 저장.
    //한 단어를 한 번에 저장해야 함

    strcpy(str_pool + index_start, yytext); // yytext를 str_pool에 저장
    index_next += length;
    str_pool[index_next++] = '\0';      //단어 뒤에 띄어쓰기 저장하고 index_next 위치 미리 옮겨둠
                                        // 단/어/저/장/됨/\0/[현재 index_next]/ 

    //hash 값 계산
    hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

    //hash table에 똑같은 단어가 저장된 선례 있나 확인  (해시값으로 찾아들어가서 strcmp로 찾아낸다)
    //있으면 찾은 항목을 HTPointer로 반환 -> 겹치므로 저장하면 안 됨
    //없으면 NULL 포인터 반환함
    HTpointer htp = lookupHT(index_start, hash_value);

    //똑같은 단어 없었던 경우
    if (htp == NULL) {

        //symtable에 1)str_pool 어디서 단어 시작하는지 2)단어 길이 저장함
        sym_table[sym_table_index][0] = line_num;
        sym_table[sym_table_index][1] = type;
        sym_table[sym_table_index][2] = index_start; //수정. 여기다 넣을 필요 없음?
        sym_table[sym_table_index++][3] = length;

        //hash table에 1)str_pool 어디서 단어 시작하는지 2)해시값 저장함
        addHT(index_start, hash_value);
        //printf("%d\t%s\n", hash_value, yytext); // 버퍼의 내용을 화면에 출력
        index_start = index_next; // 버퍼 인덱스 초기화
    }
    else { //똑같은 단어 이미 있음
        printf("%d\t%s (already exists)\n", hash_value, str_pool + index_start);
        index_next = index_start; // 버퍼에 단어 저장했던 거 초기화함 (다시 덮어쓸것!)
        flag = 1;
    }

    return 1;
}
