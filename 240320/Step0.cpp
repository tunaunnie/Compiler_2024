#include <stdio.h>

int main()
{
    FILE *fp;
    int result;
    char str_pool[100];                        // 읽은 내용을 저장할 버퍼
    result = fopen_s(&fp, "example.txt", "r"); // 파일을 읽기 모드로 열기
    //파일 open 성공하면 0 반환
    
    if (result != 0)
    {
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }
    
    while (fgets(str_pool, sizeof(str_pool), fp) != NULL) //문자열 한번에 \n까지 갖고옴.
    //더 읽을 게 없으면 fgets 함수는 NULL을 반환함.
    //읽어서 바로 str_pool에다가 저장함..
    {
        printf("%s", str_pool); // 버퍼의 내용을 화면에 출력
    }
    
    fclose(fp); // 파일 닫기
    return 0;
}