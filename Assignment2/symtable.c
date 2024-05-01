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

    // ü�̴׵� ����Ʈ�� Ž��
    while (entry != NULL) {
        if (strcmp(str_pool + (entry->index), str_pool + (id_index)) == 0) {
            return entry; // ã�� �׸� ��ȯ
        }
        entry = entry->next;
    }
    return NULL; // �׸��� ã�� ���� ���
}

void addHT(int id_index, int hscode) {
    // �� �׸� ���� �� �ʱ�ȭ
    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("�޸� �Ҵ� ����\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    if (HT[hscode] == NULL) {
        // ù ��° �׸����� �߰�
        HT[hscode] = newEntry;
    }
    else {
        // �̹� �׸��� ������, ����Ʈ�� �� �տ� �߰�
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}


void Symtable(int line_num, char* yytext, char* type) {
    int length = strlen(yytext);

    // Identifier�� ���̰� 15�ڸ� �ʰ��ϸ� ���� ó��
    if (length > 15) {
        printf("Error - Identifier length exceeds 15 characters.\n");
        index_next = index_start;
        flag = 1;
        return; // ���� ó�� �� �Լ� ����
    }

    // �ĺ��ڸ� ���ۿ� �����ϱ� ���� �����÷ο� �˻�
    if (index_next + length + 1 > sizeof(str_pool)) {
        printf("Error - String Pool Overflow\n");
        flag = 1;
        return; // �����÷ο� ó�� �� �Լ� ����
    }

    // ���ۿ� �ĺ��ڸ� ����
    strcpy(str_pool + index_start, yytext); // yytext�� str_pool�� ����
    index_next += length;
    str_pool[index_next++] = '\0';

    // �ؽð� ���
    int hash_value = divisionMethod(str_pool+index_start, HASH_TABLE_SIZE);

    // �ؽ� ���̺��� �ĺ��� �˻�
    HTpointer htp = lookupHT(index_start, hash_value);

    if (htp == NULL) {
        // �ű� �ĺ����� ���

        // �ɺ� ���̺� �ĺ��� ���� ����
        sym_table[sym_table_index][0] = index_start;
        sym_table[sym_table_index++][1] = length;

        addHT(index_start, hash_value);
        index_start = index_next; // ���� �ε��� �ʱ�ȭ

    }
    return;
    /*
    else {
        // �̹� �����ϴ� �ĺ����� ���
        printf("Error - Identifier '%s' already exists.\n", yytext);
        flag = 1;
    }
    */
}
