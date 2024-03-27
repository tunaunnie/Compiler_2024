#include <stdio.h>

int divisionMethod(char *key, int tableSize) {
    //여기서 key 인자는 해시값 구하고자 하는 하나의 단어 문자열 전체임.
    unsigned int hash_key = 0;
    int c;     //c는 정수형. 

    while (c = *key++) { //즉, 정수 c는 key 단어가 끝날때까지 (NULL일 때까지)
        hash_key += c;   //이 단어를 이루는 각 철자들의 아스키코드 값이 된다.
    }
    return hash_key % tableSize;
}

/* int divisionMethod(int key, int tableSize) {
    return key % tableSize;
} */

int midsquareMethod(char *key, int tableSize) {
    int squared;
    int midPart;
    unsigned int hash_key = 0;
    int c;

    while (c = *key++) {
        hash_key += c;
    }

    squared = hash_key * hash_key;
    midPart = squared / 100 % 1000;
    return midPart % tableSize;
}

int foldingMethod(char* key, int tableSize) {
    int fold = 0;
    unsigned int hash_key = 0;
    int c;

    while (c= *key++) {
        hash_key += c;
    }


}