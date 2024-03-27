#include <stdio.h>

int divisionMethod(int key, int tableSize) {
	return key % tableSize;
}

/*
int divisionMethod(char* key, int tableSize) { //함수의 파라미터 문자열로 변환
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
	midPart = squared / 100 % 1000; //제곱값의 중간 부분 추출
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