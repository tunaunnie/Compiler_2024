#include <stdio.h>
#include <string.h>

int string_find(char* str, char ch){
    int index = 0;

    while(str[index] != NULL){
        if (str[index] == ch){
            return 1;
        }
        index++;
    }
    return 0;
}

int main(){

    char str[] = "1234abcd";

    printf("%d", str.find("abc"));

    return 0;
}