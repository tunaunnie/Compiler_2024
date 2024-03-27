#include <stdio.h>

int divisionMethod(int key, int tableSize) {
	return key % tableSize;
}

/*
int divisionMethod(char* key, int tableSize) { //�Լ��� �Ķ���� ���ڿ��� ��ȯ
	unsigned int hash_key = 0;
	int c;

	while (c = *key++) {
		hash_key += c;
	}
	return hash_key % tableSize;
}
*/

int midsquareMethod(char* key, int tableSize) {
	int squared;
	int midPart;
	unsigned int hash_key = 0;
	int c;

	while (c = *key++) {
		hash_key += c;
	}
	
	squared = hash_key * hash_key;
	midPart = squared / 100 % 1000; //�������� �߰� �κ� ����
	return midPart % tableSize;
}

int foldingMethod(char* key, int tableSize) {
	int fold = 0;
	unsigned int hash_key = 0;
	int c;

	while (c = *key++) {
		hash_key += c;
	}

}