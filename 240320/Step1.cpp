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


int main()
{
    FILE *fp;
    int result;
    char str_pool[100];                        // identifier 기준으로 저장할 string pool
    char separators[] = " ,;\t\n\r\n";
    int index_start = 0;
    int index_next = 0;

    result = fopen_s(&fp, "example.txt", "r"); // 파일을 읽기 모드로 열기
    //파일 open 성공하면 0 반환
    if (result != 0)
    {
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }
    
    char ch;
    do {                    //fp 파일에서 읽을 게 있는 한 계속 반복
        ch = fgetc(fp);

        while (strchr(separators, ch) != NULL){

            /* if(지금 읽은 문자가 digit라면)
                error처리, return  */

        }



    } while (ch != EOF);

    
    fclose(fp); // 파일 닫기
    return 0;
}