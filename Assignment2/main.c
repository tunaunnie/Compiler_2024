/*
[ COMPILER ASSIGNMENT 02 _ CONTRIBUTION ]

1. 2076362 JAEUN JEON (33%)
2. 2076316 YEEUN LEE (33%)
3. 2176211 JIWON YANG (33%)
*/


/*
 [ COMPILER ASSIGNMENT 02 _ REQUIRED FUNCTIONS ]

 1. 각 identifier가 인식되면, Symbol Table에서 identifier를 관리한다.
    1) identifier의 string_pool 시작 인덱스와 length를 저장.
    2) printST에서는 등록된 indentifier의 시작 인덱스, 길이, 내용을 출력.


 2. Source program에 에러가 있는 경우, 가능한 명확하게 error message를 프린트한다.
    1) 이미 저장된 identifier의 경우, Symbol Table에 저장하지 않음.

    2) 15자 이상의 identifier의 경우, 에러 메시지를 출력하고 Symbol Table에 저장하지 않음.
       : "Error - Identifier length exceeds 15 characters."

    3) String Pool이 가득 찬 경우, Overflow 메시지를 출력하고 해당 identifier는 Symbol Table에 저장하지 않음.
       : "Error - String Pool Overflow (The token just below this line is not stored in the string pool)"

    4) 숫자로 시작하는 identifier의 경우, Symbol Table에 저장하지 않음.
       : "Error at line %d: start with digit."

    5) 허용하지 않는 문자를 포함하고 있는 경우, Symbol Table에 저장하지 않음.
        : "Error at line %d: %s is not valid."
        : '허용하지 않는 문자' 는 ADDIGIONAL DEFINITIONS에서 정의한다.


 3. 각 토큰이 인식되었을 때, 토큰의 내용을 프린트한다.
    0) printtoken()은 Source program에 포함된 토큰을 모두 인식하여 출력한다. (overflow 발생 여부, symbol table 저장 여부 무관)
    1) 식별자인 경우 : line number, token type, token, ST_index 출력
    2) 식별자가 아닌 경우 : line number, token type, token 출력
 */


 /*
  [ COMPILER ASSIGNMENT 02 _ ADDITIONAL DEFINITIONS ]

  1. 예약어 (keyword) 정의
    : "const", "else", "if", "int", "float", "char", "string", "return", "void", "while", "main", "print", "printf"

  2. 연산자 (operator) 정의
   2-1. 사칙 연산자: "+", "-", "*", "/", "%"
   2-2. 배정 연산자: "=", "+=", "-=", "*=", "/=", "%="
   2-3. 논리 연산자: "!", "&&", "||"
   2-4. 관계 연산자: "==", "!=", "<", ">", "<=", ">="
   2-5. 증감 연산자: "++", "--"

  3. Special Symbols 정의
    : "(", ")", ".", ",", "{", "}", "[", "]", ";", \"

  4. illegal character 정의
   4-1. 허용되지 않은 문자는 `~@#$^ 로 정의한다.
   4-2. 허용하지 않는 문자가 포함된 문자열은 해당 문자열 전체를 에러로 처리한다.
   4-3. 연산자에 해당하는 문자는 에러 처리하지 않는다. 문법에 어긋나는 표현은 차후 의미분석기 구현 시 수정할 예정

  5. 숫자(실수형) 처리 - TFLOATNUM
    5-1. 고정소수점을 사용하는 경우는 고려하지 않는다.
    5-2. 부동소수점을 사용하는 경우, 숫자로 인식한다.

  6. 숫자(정수형) 처리 - TINTNUM
    6-1. 정수형 숫자는 10진수, 8진수, 16진수 형태를 인식한다.

 */

#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void initST() {
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        sym_table[i][0] = -1;
        sym_table[i][1] = NULL;
    }
}

void printST() {
    printf("\n\n\nSymbol Table:\n");
    int i;
    printf("[ ST_Index ] \t [ Length ] \t [ Identifier ]\n");
    printf("----------------------------------------------------------\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1) {
            printf("%-16d %-16d %-16s\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][0]);
        }
    }
}

void printtoken(int line_num, char* token_type, char* token_name, int sym_index_num) {
    if (sym_index_num == -1) {
        // 예약어인 경우 (심볼 인덱스가 -1)
        printf("%-16d %-16s %-16s\n", line_num, token_type, token_name);
    }
    else {
        // 식별자인 경우 (심볼 인덱스가 유효한 경우)
        printf("%-16d %-16s %-16s %-16d\n", line_num, token_type, token_name, sym_table[sym_index_num][0]);
    }
}

void printHT() {
    printf("\nHash Table:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTpointer entry = HT[i];
        if (entry != NULL) {
            printf("[%d]: ", i);
            while (entry != NULL) {
                printf("%d -> ", entry->index);
                entry = entry->next;
            }
            printf("NULL\n");
        }
    }
}

int main()
{
    enum tokentypes tn;  // token types
    initST();

    int line_num = 1;

    printf("\n\t\t\t < Tokens of Mini C > \t\t\t\n");
    printf("[line #] \t [type] \t [token] \t [ST index] (only TIDENT)\n");
    printf("---------------------------------------------------------------------------\n");

    while ((tn = yylex()) != TEOF) {
        switch (tn) {
        case LINEPROCESS: line_num++; break;
        case TCONST: printtoken(line_num, "TCONST", yytext, -1); break;             // const
        case TELSE: printtoken(line_num, "TELSE", yytext, -1); break;               // else
        case TIF: printtoken(line_num, "TIF", yytext, -1); break;                   // if
        case TINT: printtoken(line_num, "TINT", yytext, -1); break;                 // int
        case TFLOAT: printtoken(line_num, "TFLOAT", yytext, -1); break;             // float
        case TCHAR: printtoken(line_num, "TCHAR", yytext, -1); break;               // char
        case TSTRING: printtoken(line_num, "TSTRING", yytext, -1); break;           // string
        case TRETURN: printtoken(line_num, "TRETURN", yytext, -1); break;           // return
        case TVOID: printtoken(line_num, "TVOID", yytext, -1); break;               // void
        case TWHILE: printtoken(line_num, "TWHILE", yytext, -1); break;             // while
        case TMAIN: printtoken(line_num, "TMAIN", yytext, -1); break;               // main
        case TPRINT: printtoken(line_num, "TPRINT", yytext, -1); break;             // print, printf

        case TPLUS: printtoken(line_num, "TPLUS", yytext, -1); break;               // +
        case TMINUS: printtoken(line_num, "TMINUS", yytext, -1); break;             // -
        case TMUL: printtoken(line_num, "TMUL", yytext, -1); break;               // *
        case TDIV: printtoken(line_num, "TDIV", yytext, -1); break;               // /
        case TMOD: printtoken(line_num, "TMOD", yytext, -1); break;                 // %

        case TASSIGN: printtoken(line_num, "TASSIGN", yytext, -1); break;           // =
        case TADDASSIGN: printtoken(line_num, "TADDASSIGN", yytext, -1); break;     // +=
        case TSUBASSIGN: printtoken(line_num, "TSUBASSIGN", yytext, -1); break;     // -=
        case TMULASSIGN: printtoken(line_num, "TMULASSIGN", yytext, -1); break;     // *=
        case TDIVASSIGN: printtoken(line_num, "TDIVASSIGN", yytext, -1); break;     // /=
        case TMODASSIGN: printtoken(line_num, "TMODASSIGN", yytext, -1); break;     // %=

        case TNOT: printtoken(line_num, "TNOT", yytext, -1); break;                 // !
        case TAND: printtoken(line_num, "TAND", yytext, -1); break;                 // &&
        case TOR: printtoken(line_num, "TOR", yytext, -1); break;                   // ||

        case TEQUAL: printtoken(line_num, "TEQUAL", yytext, -1); break;             // ==
        case TNOTEQU: printtoken(line_num, "TNOTEQU", yytext, -1); break;           // !=
        case TLESS: printtoken(line_num, "TLESS", yytext, -1); break;               // <
        case TGREAT: printtoken(line_num, "TGREAT", yytext, -1); break;             // >
        case TLESSE: printtoken(line_num, "TLESSE", yytext, -1); break;               // <=
        case TGREATE: printtoken(line_num, "TGREATE", yytext, -1); break;             // >=

        case TINC: printtoken(line_num, "TINC", yytext, -1); break;                 // ++
        case TDEC: printtoken(line_num, "TDEC", yytext, -1); break;                 // --
       
        case TLPAREN: printtoken(line_num, "TLPAREN", yytext, -1); break;             // (
        case TRPAREN: printtoken(line_num, "TRPAREN", yytext, -1); break;             // )
        case TDOT: printtoken(line_num, "TDOT", yytext, -1); break;             // .
        case TCOMMA: printtoken(line_num, "TCOMMA", yytext, -1); break;             // ,
        case TLBRACE: printtoken(line_num, "TLBRACE", yytext, -1); break;           // {
        case TRBRACE: printtoken(line_num, "TRBRACE", yytext, -1); break;           // }
        case TLBRACKET: printtoken(line_num, "TLBRACKET", yytext, -1); break;           // [
        case TRBRACKET: printtoken(line_num, "TRBRACKET", yytext, -1); break;           // ]
        case TSEMICOLON: printtoken(line_num, "TSEMICOLON", yytext, -1); break;     // ;
        case TQUOTE: printtoken(line_num, "TQUOTE", yytext, -1); break;     // "
        
        case TIDENT: {                                                              // identifier
            //식별자인 경우만 sym_table에 저장. 식별자 저장 후, 그거랑 별개로 매 줄마다 출력하는 printtoken 해줌
            Symtable(line_num, yytext, "TIDENT");
            //신규 식별자인 경우
            if (flag == 0)
            {
                printtoken(line_num, "TIDENT", yytext, sym_table_index - 1);  break;
            }
            //overflow인 경우
            else if (flag == 2) {
                printtoken(line_num, "TIDENT", yytext, -1); break;
            }
            //이미 존재하는 식별자의 경우
            else if (flag == 3) {
                printtoken(line_num, "TIDENT", yytext, temp_index); break;
            }
            //15자 초과인 경우
            else { flag = 0; break; }
        }
        case TINTNUM: printtoken(line_num, "TINTNUM", yytext, -1); break;           
        case TFLOATNUM: printtoken(line_num, "TFLOATNUM", yytext, -1); break;

        case TERROR1: reporterror1(line_num, yytext); break;                        // 숫자로 시작하는 에러 처리
        case TERROR2: reporterror2(line_num, yytext); break;                        //illegal 문자 에러 처리   
        case TERROR3: reporterror3(line_num, yytext); break;                        //나머지 illegal 문자 에러 처리
        }
    }

    printST();
    printHT();
    return 0;
}