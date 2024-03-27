#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "STMain.h"
#include "HashFunc.h"

char separators[] = " ,;\t\n\r\n"; //구분자
char str_pool[100]; //string pool
int sym_table[SYM_TABLE_SIZE][2]; //symbol table 2차원 배열 생성
/*int pool_size = 0;

bool isStringPoolFull() {
	return pool_size >= STRING_POOL_SIZE;
}
*/
//실습 4 추가부분
typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;
	HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];

HTpointer lookup_hash_table(int id_index, int hscode) {
	HTpointer entry = HT[hscode];

	while (entry != NULL) {
		if (strcmp(str_pool + sym_table[entry->index][0], str_pool + (id_index)) == 0) {/* HT에서 찾고자 하는 값 비교 */
			return entry; //같으면 찾은 항목 반환
		}
		entry = entry->next;
	}
	return NULL; //항목을 찾지 못한 경우
}

void add_hash_table(int id_index, int hscode) {
	//새 항목 생성 및 초기화
	HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
	if (newEntry == NULL) {
		printf("메모리 할당 실패\n");
		exit(1);
	}
	newEntry->index = id_index;
	newEntry->next = NULL;

	/*HT에 추가*/
	if (HT[hscode] == NULL) { //아무것도 없는 상태라면
		HT[hscode] = newEntry; //새로운 entry로 추가
	}
	else { //만약 이미 있다면 후위법으로 추가
		newEntry->next = HT[hscode];
		HT[hscode] = newEntry;
	}
}
//여기까지 실습 4에 추가된 부분

void init_sym_table() { //symbol table 초기화(비어있는 TABLE인지 아닌지 확인)
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
		if (sym_table[i][0] != -1) { //symbol table에 값이 있으면
			printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]); //출력
		}
	}
}


int main() {
	FILE* fp;
	int result;
	int c; //읽은 문자를 저장할 변수
	int index_start = 0; //현재 들어온 identifier의 첫번째 위치
	int index_next = 0; //다음에 들어올 identifier가 저장되어야하는 위치
	int index = 0;
	int hash_value = -1;


	init_sym_table();

	result = fopen_s(&fp, "example.txt", "r"); //파일을 읽기 모드로 열기
	if (result != 0) {
		printf("파일 열기 실패(%d)\n", result);
		return -1;
	}

	while ((c = fgetc(fp)) != EOF) { //파일 끝까지 문자 읽기 
		/*추가한 부분임...
		if(isStringPoolFull()) {
			printf("...Error...OVERFLOW...");
			break;
		}
		여기까지..*/

		if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1) { //구분자를 만나거나 버퍼 크기 제한에 도달했을 때
			if (index_start < index_next) { //버퍼에 내용이 있을 경우만 출력
				str_pool[index_next] = '\0'; //문자열 종료

				/* 15자 안 넘게 추가한 부분*/
				int str_len = index_next - index_start; //들어온 문자열 길이 계산
				if (str_len > MAX_STRING_LENGTH) { //15자 보다 식별자 길이가 크다면
					printf("...Error... Inserted String is longer than 15 words...");

					while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //이 문자열은 저장하지 않고 건너뜀.
					index_next = index_start; //인덱스 초기화
					continue; //다음 문자로 이동해서 계속 진행함.
				}

				str_pool[index_next] = '\0';
				//여기까지...

				if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9') {//문자열이 숫자로 시작하는 경우
					printf("Error - start with digit (%s)\n", str_pool + index_start); //에러 출력
					index_next = index_start; //버퍼 인덱스 초기화
				}
				else { //실습 2 수정된 파트 : symbol table에 identifier 추가
					sym_table[index][0] = index_start;
					sym_table[index++][1] = (int)strlen(str_pool + index_start);

					hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);
					//hash 값 계산한 다음 hashtable에 추가

					//우선 동일한 hash값을 가지는 identifier(entry)가 있는지 봐야함.

					HTpointer htp = lookup_hash_table(index - 1, hash_value);
					if (htp == NULL) {
						add_hash_table(index - 1, hash_value);
						printf("%s (Hash: %d) \n", str_pool + sym_table[index - 1][0], hash_value); //버퍼의 내용을 화면에 출력
						index_start = ++index_next; //버퍼 인덱스 초기화
					}
					else {
						printf("%s (Already exists. Hash : %d) \n", str_pool + sym_table[index - 1][0], hash_value); //버퍼의 내용을 화면에 출력
						index_start = ++index_next; //버퍼 인덱스 초기화
					}
				}
			}

			if (strchr(separators, c) == NULL) { //구분자가 아닌 문자인 경우
				str_pool[index_next++] = (char)c; //string pool에 저장
			}
			continue; //다음 문자로 이동
		}
		str_pool[index_next++] = (char)c; //string pool에 문자 저장
	}

	if (index_start < index_next) { //마지막 문자열 출력
		str_pool[index_next] = "\0"; //문자열 종료
		//실습2에서 추가된 파트  : symbol table에 identifier 추가
		sym_table[index][0] = index_start;
		sym_table[index++][1] = (int)strlen(str_pool + index_start);

		hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

		HTpointer htp = lookup_hash_table(index_start, hash_value);
		if (htp == NULL) {
			add_hash_table(index_start, hash_value);
			printf("%s (Hash: %d) \n", str_pool + index_start, hash_value); //버퍼의 내용을 화면에 출력
		}
		else {
			printf("%s (Already exists. Hash : %d) \n", str_pool + index_start, hash_value); //버퍼의 내용을 화면에 출력
		}
	}

	print_sym_table();

	fclose(fp); //파일 닫기
	return 0;

}
