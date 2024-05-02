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


// �ؽ� ���̺��� �׸��� ã�� �Լ�
HTpointer lookup_hash_table(int id_index, int hscode) {
    HTpointer entry = HT[hscode];

    while (entry != NULL) {
        if (strcmp(str_pool + sym_table[entry->index][0], str_pool + id_index) == 0) {
            return entry; // ã�� �׸� ��ȯ
        }
        entry = entry->next;
    }
    return NULL; // �׸��� ã�� ���� ���
}

void add_hash_table(int id_index, int hscode) {

    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    // Hash Table�� �߰�
    if (HT[hscode] == NULL) {
        HT[hscode] = newEntry;
    }
    else {
        // �Ĺ������ �߰�
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

void print_sym_table() { //symbol table ����ϴ� �Լ�
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

    result = fopen_s(&fp, "example.txt", "r"); // "example.txt" ���� ����
    if (result != 0) {
        printf("���� ���� ����(%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF) { // ���� ������ ���� �б�

        if (index_next >= sizeof(str_pool) - 1) {
            printf("Error - String Pool ũ�⸦ �ʰ��߽��ϴ�.\n");
            while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //�� ���ڿ��� �������� �ʰ� �ǳʶ�.
            index_next = index_start; //�ε��� �ʱ�ȭ
            //continue; //���� ���ڷ� �̵��ؼ� ��� ������.
            fclose(fp); // ���� �ݱ�
            break; // ���α׷� ����
        }

        else if (strchr(separators, c) != NULL) {
            // �����ڸ� �����ų� ���� ũ�� ���ѿ� �������� ��
            if (index_start < index_next) { // ���ۿ� ������ ���� ��츸 ���
                str_pool[index_next] = '\0'; // ���ڿ� ����

                /* 15�� �� �Ѱ� �߰��� �κ�*/
                int str_len = index_next - index_start; //���� ���ڿ� ���� ���
                if (str_len > MAX_STRING_LENGTH) { //15�� ���� �ĺ��� ���̰� ũ�ٸ�
                    printf("Error - Inserted String is longer than 15 words");

                    while ((c = fgetc(fp)) != EOF && strchr(separators, c) == NULL) {} //�� ���ڿ��� �������� �ʰ� �ǳʶ�.
                    index_next = index_start; //�ε��� �ʱ�ȭ
                    continue; //���� ���ڷ� �̵��ؼ� ��� ������.
                }

                str_pool[index_next] = '\0';

                if (str_pool[index_start] >= '0' && str_pool[index_start] <= '9') { // ���ڿ��� ���ڷ� ������ ���
                    printf("Error - start with digit (%s)\n", str_pool + index_start); // ���� ���
                    index_next = index_start; // ���� �ε��� �ʱ�ȭ
                }
                else {
                    int i = 0;
                    int hash_key = 0;
                    sym_table[index][0] = index_start;
                    sym_table[index++][1] = (int)strlen(str_pool + index_start);

                    while (str_pool[index_start + i] != '\0') { //�������� NULL�� �ƴ� ���
                        hash_key += str_pool[index_start + i++];
                    }
                    hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE);

                    HTpointer htp = lookup_hash_table(index_start, hash_value);
                    if (htp == NULL) {
                        add_hash_table(index - 1, hash_value);
                        printf("%s (Hash: %d)\n", str_pool + sym_table[index - 1][0], hash_value); // ������ ������ ȭ�鿡 ���
                        index_start = ++index_next; // ���� �ε��� �ʱ�ȭ
                    }
                    else {
                        printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value); // ������ ������ ȭ�鿡 ���
                        index_start = ++index_next; // ���� �ε��� �ʱ�ȭ
                    }
                }
            }
            if (strchr(separators, c) == NULL) {
                str_pool[index_next++] = (char)c; // �����ڰ� �ƴ� ���ڸ� ���ۿ� ����
            }
            continue; // ���� ���ڷ� �̵�
        }
        // ���� ���ڰ� ������ ���� ������ ���. �߰����� ������ ���ƾ� ��.
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_')))
        {
            printf("Error - usuage of unable character : %c\n", c);
            while ((c = fgetc(fp)) != EOF && !(strchr(separators, c) != NULL)) {
            }
            index_next = index_start;
            continue;
        }
        // ���� ���ڰ� �����ڰ� �ƴ� ���� ������ ���
        else
            str_pool[index_next++] = (char)c;
    }

    if (index_start < index_next) { // ������ ���ڿ� ���
        int i = 0;
        int hash_key = 0;

        str_pool[index_next] = '\0'; // ���ڿ� ����
        sym_table[index][0] = index_start;
        sym_table[index++][1] = (int)strlen(str_pool + index_start);

        while (str_pool[index_start + i] != '\0') {
            hash_key += str_pool[index_start + i++];
        }
        hash_value = divisionMethod(hash_key, HASH_TABLE_SIZE);


        HTpointer htp = lookup_hash_table(index_start, hash_value);
        if (htp == NULL) {
            add_hash_table(index_start, hash_value);
            printf("%s (Hash: %d)\n", str_pool + index_start, hash_value); // ������ ������ ȭ�鿡 ���
        }
        else {
            printf("%s (Already exists. Hash: %d)\n", str_pool + index_start, hash_value); // ������ ������ ȭ�鿡 ���
        }

    }

    print_sym_table();
    print_hash_table();

    fclose(fp); // ���� �ݱ�
    return 0;
}