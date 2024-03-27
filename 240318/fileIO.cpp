#include <stdio.h>

int main()
{
    FILE *fp;
    int result;
    char str_pool[100];                        // 읽은 내용을 저장할 버퍼
    result = fopen_s(&fp, "example.txt", "r"); // 파일을 읽기 모드로 열기
    
    if (result != 0)
    {
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }
    
    while (fgets(str_pool, sizeof(str_pool), fp) != NULL) 
    //파일의 끝에 도달하면 fgets는 NULL 포인터를 반환함
    {
        printf("%s", str_pool); // 버퍼의 내용을 화면에 출력
    }
    
    fclose(fp); // 파일 닫기
    return 0;
}
