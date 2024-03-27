#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char separators[] = ",;\t\n\r\n ";
char str_pool[100];

int main(){
    FILE* fp;
    int result;
    int c; //읽은 문자를 저장할 변수
    int index_start = 0;
    int index_next = 0;

    result = fopen_s(&fp, "example.txt", "r");
    if (result != 0){
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }

    while ((c = fgetc(fp)) != EOF ){ //파일 끝까지 문자 읽기
        if (strchr(separators, c) != NULL || index_next >= sizeof(str_pool) - 1){
            //구분자를 만나거나, 버퍼 크기 제한에 도달했을 때.
            // 이제 한 단어를 출력해야 함. 하지만 몇 가지 예외 상황 고려 필요
            
            if (index_start < index_next) { //구분자 나왔으니 출력할건데, 버퍼에 내용이 있어야만 출력 가능
                str_pool[index_next] = '\0'; //스트링풀에는 문자 단위마다 띄워서 저장해야 하니까 한 단어 종료표시 넣음

                if (str_pool[index_next] >= '0' && str_pool[index_next]<= '9') { 
                    //문자열이 숫자인 경우. 다음 단어가 숫자로 시작하는지 확인.
                    //start index만 확인하면 되니까 구분자를 만난 경우의 if문 안으로 들어옴
                    printf("Error - start with digit (%s)\n", str_pool + index_start);
                    index_next = index_start; //버퍼 인덱스 초기화. 에러 나기 전 단어까지만 출력
                }
                else {
                    printf("%s\n", str_pool + index_start); //스트링풀의 한 단어를 출력함.
                    index_start = ++index_next; //버퍼 인덱스 초기화
                }
            }

            if (strchr(separators, c) == NULL) { //구분자가 아니라 버퍼 크기 제한 때문에 들어온 경우면
                str_pool[index_next++] = (char)c; //일단 구분자가 아닌 문자를 버퍼에 저장
            }

            continue; //다음 문자로 이동. <<이 continue 때문에 버퍼에꽉찬경우, EOF 나올때까지 계속반복하게 되는건가?
        }

        str_pool[index_next++] = (char)c; //버퍼에 문자 저장
    }
        
    if (index_start < index_next) {  //마지막 문자열 출력
        str_pool[index_next] = '\0';
        printf("%s\n", str_pool + index_start); //버퍼의 내용을 화면에 출력
    }

    fclose(fp);
    return 0;
}