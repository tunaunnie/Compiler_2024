#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "STMain.h"
#include "HashFunc.h"

char separators[] = " ,;\t\n\r\n"; //������
char str_pool[100]; //string pool
int sym_table[SYM_TABLE_SIZE][2]; //symbol table 2���� �迭 ����
/*int pool_size = 0;

bool isStringPoolFull() {
	return pool_size >= STRING_POOL_SIZE;
}
*/
//�ǽ� 4 �߰��κ�
typedef struct HTentry* HTpointer;
typedef struct HTentry {
	int index;
	HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];

HTpointer lookup_hash_table(int id_index, int hscode) {
	HTpointer entry = HT[hscode];

	while (entry != NULL) {
		if (strcmp(str_pool + sym_table[entry->index][0], str_pool + (id_index)) == 0) {/* HT���� ã���� �ϴ� �� �� */
			return entry; //������ ã�� �׸� ��ȯ
		}
		entry = entry->next;
	}
	return NULL; //�׸��� ã�� ���� ���
}

void add_hash_table(int id_index, int hscode) {
	//�� �׸� ���� �� �ʱ�ȭ
	HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
	if (newEntry == NULL) {
		printf("�޸� �Ҵ� ����\n");
		exit(1);
	}
	newEntry->index = id_index;
	newEntry->next = NULL;

	/*HT�� �߰�*/
	if (HT[hscode] == NULL) { //�ƹ��͵� ���� ���¶��
		HT[hscode] = newEntry; //���ο� entry�� �߰�
	}
	else { //���� �̹� �ִٸ� ���������� �߰�
		newEntry->next = HT[hscode];
		HT[hscode] = newEntry;
	}
}
//������� �ǽ� 4�� �߰��� �κ�

void init_sym_table() { //symbol table �ʱ�ȭ(����ִ� TABLE���� �ƴ��� Ȯ��)
	int i;
	for (i = 0; i < SYM_TABLE_SIZE; i++) {
		sym_table[i][0] = -1;
		sym_table[i][1] = -1;
	}
}

void print_sym_table() { //symbol table ����ϴ� �Լ�
	int i;
	printf("\nSymbol Table\n");
	printf("Index\tLength\tSymbol\n");
	for (i = 0; i < SYM_TABLE_SIZE; i++) {
		if (sym_table[i][0] != -1) { //symbol table�� ���� ������
			printf("%d\t%d\t%s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]); //���
		}
	}
}


int main() {
	FILE* fp;
	int result;
	int c; //���� ���ڸ� ������ ����
	int index_start = 0; //���� ���� identifier�� ù��° ��ġ
	int index_next = 0; //������ ���� identifier�� ����Ǿ���ϴ� ��ġ
	int index = 0;
	int hash_value = -1;


	init_sym_table();

	result = fopen_s(&fp, "example.txt", "r"); //������ �б� ���� ����
	if (result != 0) {
		printf("���� ���� ����(%d)\n", result);
		return -1;
	}

	while ((c = fgetc(fp)) != EOF) { //���� ������ ���� �б� 
		/*�߰��� �κ���...
		if(isStringPoolFull()) {
			printf("...Error...OVERFLOW...");
			break;
		}
		�������..*/

		if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1) { //�����ڸ� �����ų� ���� ũ�� ���ѿ� �������� ��
			if (index_start < index_next) { //���ۿ� ������ ���� ��츸 ���
				str_pool[index_next] = '\0'; //���ڿ� ����

				/* 15�� �� �Ѱ� �߰��� �κ�*/
				int str_len = index_next - index_start; //���� ���ڿ� ���� ���
				if (str_len > MAX_STRING_LENGTH) { //15�� ���� �ĺ��� ���̰� ũ�ٸ�
					printf("...Error... Inserted String is longer than 15 words...");

					while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //�� ���ڿ��� �������� �ʰ� �ǳʶ�.
					index_next = index_start; //�ε��� �ʱ�ȭ
					continue; //���� ���ڷ� �̵��ؼ� ��� ������.
				}

				str_pool[index_next] = '\0';
				//�������...

				if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9') {//���ڿ��� ���ڷ� �����ϴ� ���
					printf("Error - start with digit (%s)\n", str_pool + index_start); //���� ���
					index_next = index_start; //���� �ε��� �ʱ�ȭ
				}
				else { //�ǽ� 2 ������ ��Ʈ : symbol table�� identifier �߰�
					sym_table[index][0] = index_start;
					sym_table[index++][1] = (int)strlen(str_pool + index_start);

					hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);
					//hash �� ����� ���� hashtable�� �߰�

					//�켱 ������ hash���� ������ identifier(entry)�� �ִ��� ������.

					HTpointer htp = lookup_hash_table(index - 1, hash_value);
					if (htp == NULL) {
						add_hash_table(index - 1, hash_value);
						printf("%s (Hash: %d) \n", str_pool + sym_table[index - 1][0], hash_value); //������ ������ ȭ�鿡 ���
						index_start = ++index_next; //���� �ε��� �ʱ�ȭ
					}
					else {
						printf("%s (Already exists. Hash : %d) \n", str_pool + sym_table[index - 1][0], hash_value); //������ ������ ȭ�鿡 ���
						index_start = ++index_next; //���� �ε��� �ʱ�ȭ
					}
				}
			}

			if (strchr(separators, c) == NULL) { //�����ڰ� �ƴ� ������ ���
				str_pool[index_next++] = (char)c; //string pool�� ����
			}
			continue; //���� ���ڷ� �̵�
		}
		str_pool[index_next++] = (char)c; //string pool�� ���� ����
	}

	if (index_start < index_next) { //������ ���ڿ� ���
		str_pool[index_next] = "\0"; //���ڿ� ����
		//�ǽ�2���� �߰��� ��Ʈ  : symbol table�� identifier �߰�
		sym_table[index][0] = index_start;
		sym_table[index++][1] = (int)strlen(str_pool + index_start);

		hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

		HTpointer htp = lookup_hash_table(index_start, hash_value);
		if (htp == NULL) {
			add_hash_table(index_start, hash_value);
			printf("%s (Hash: %d) \n", str_pool + index_start, hash_value); //������ ������ ȭ�鿡 ���
		}
		else {
			printf("%s (Already exists. Hash : %d) \n", str_pool + index_start, hash_value); //������ ������ ȭ�鿡 ���
		}
	}

	print_sym_table();

	fclose(fp); //���� �ݱ�
	return 0;

}
