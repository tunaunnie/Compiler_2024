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
    // �����ؾ� �� Identifier�� yytext ���·� ����Ǿ� ����.
    // ���ڽ���, illegal ident ���� ������ ��� ����.
    // 15�� �̻� ó��, �����÷ο� ó���� �ϸ� ��
    // '������ ���ڿ� ���' ���� �� �ʿ䰡 ���°ǰ�?

    int length = strlen(yytext);            //�Է��� number�� ��� ������ (15�� �̻��� �ɷ� ��� ..)

    //�����÷ο� ó��. �Ѿ�� return�ؼ� ��������
    //�� ���ھ� �ִ� �� �ƴϰ� yytext �ܾ �Ѳ����� �ִ� ��. �ѹ��� ������ ���̷� ��
    if (index_next + length > sizeof(str_pool)) {     //-1 ���ص� �ǰ���..?

        printf("String Pool Overflow\n");
        printST();
        printHT();
        flag = 1;

        return 1;
    }

    //15�� �̻� ������ ó��
    if (length > 15) {
        printf("Error - Exceed\n");
        index_next = index_start;
        flag = 1;

        return 1;
    }

    //15�� �̻� �ƴϰ� �����÷ο쵵 �� ���� ���ۿ� �ܾ� ����.
    //�� �ܾ �� ���� �����ؾ� ��

    strcpy(str_pool + index_start, yytext); // yytext�� str_pool�� ����
    index_next += length;
    str_pool[index_next++] = '\0';      //�ܾ� �ڿ� ���� �����ϰ� index_next ��ġ �̸� �Űܵ�
                                        // ��/��/��/��/��/\0/[���� index_next]/ 

    //hash �� ���
    hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

    //hash table�� �Ȱ��� �ܾ ����� ���� �ֳ� Ȯ��  (�ؽð����� ã�Ƶ��� strcmp�� ã�Ƴ���)
    //������ ã�� �׸��� HTPointer�� ��ȯ -> ��ġ�Ƿ� �����ϸ� �� ��
    //������ NULL ������ ��ȯ��
    HTpointer htp = lookupHT(index_start, hash_value);

    //�Ȱ��� �ܾ� ������ ���
    if (htp == NULL) {

        //symtable�� 1)str_pool ��� �ܾ� �����ϴ��� 2)�ܾ� ���� ������
        sym_table[sym_table_index][0] = line_num;
        sym_table[sym_table_index][1] = type;
        sym_table[sym_table_index][2] = index_start; //����. ����� ���� �ʿ� ����?
        sym_table[sym_table_index++][3] = length;

        //hash table�� 1)str_pool ��� �ܾ� �����ϴ��� 2)�ؽð� ������
        addHT(index_start, hash_value);
        //printf("%d\t%s\n", hash_value, yytext); // ������ ������ ȭ�鿡 ���
        index_start = index_next; // ���� �ε��� �ʱ�ȭ
    }
    else { //�Ȱ��� �ܾ� �̹� ����
        printf("%d\t%s (already exists)\n", hash_value, str_pool + index_start);
        index_next = index_start; // ���ۿ� �ܾ� �����ߴ� �� �ʱ�ȭ�� (�ٽ� �����!)
        flag = 1;
    }

    return 1;
}
