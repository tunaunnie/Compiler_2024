#include <stdio.h>

int divisionMethod(char* key, int tableSize) { //제산법
	unsigned int hash_key = 0;
	int c;

	while (c = *key++) {
		hash_key += c;
	}
	return hash_key % tableSize;
}

int midsquareMethod(char* key, int tableSize) { //제곱값 중간법
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

