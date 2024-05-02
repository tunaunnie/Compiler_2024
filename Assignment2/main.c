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
    printf("\[line #] \t [type] \t [token] \t [ST index] (only TIDENT)\n");
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
        case TRETURN: printtoken(line_num, "TRETURN", yytext, -1); break;           // return
        case TVOID: printtoken(line_num, "TVOID", yytext, -1); break;               // void
        case TWHILE: printtoken(line_num, "TWHILE", yytext, -1); break;             // while
        case TEQUAL: printtoken(line_num, "TEQUAL", yytext, -1); break;             // ==
        case TNOTEQU: printtoken(line_num, "TNOTEQU", yytext, -1); break;           // !=
        case TLESS: printtoken(line_num, "TLESS", yytext, -1); break;               // <=
        case TGREAT: printtoken(line_num, "TGREAT", yytext, -1); break;             // >=
        case TAND: printtoken(line_num, "TAND", yytext, -1); break;                 // &&
        case TOR: printtoken(line_num, "TOR", yytext, -1); break;                   // ||
        case TINC: printtoken(line_num, "TINC", yytext, -1); break;                 // ++
        case TDEC: printtoken(line_num, "TDEC", yytext, -1); break;                 // --
        case TADDASSIGN: printtoken(line_num, "TADDASSIGN", yytext, -1); break;     // +=
        case TSUBASSIGN: printtoken(line_num, "TSUBASSIGN", yytext, -1); break;     // -=
        case TMULASSIGN: printtoken(line_num, "TMULASSIGN", yytext, -1); break;     // *=
        case TDIVASSIGN: printtoken(line_num, "TDIVASSIGN", yytext, -1); break;     // /=
        case TMODASSIGN: printtoken(line_num, "TMODASSIGN", yytext, -1); break;     // %=
        case TPLUS: printtoken(line_num, "TPLUS", yytext, -1); break;               // +
        case TMINUS: printtoken(line_num, "TMINUS", yytext, -1); break;             // -
        case TSTAR: printtoken(line_num, "TSTAR", yytext, -1); break;               // *
        case TMOD: printtoken(line_num, "TMOD", yytext, -1); break;                 // /
        case TASSIGN: printtoken(line_num, "TASSIGN", yytext, -1); break;           // =
        case TPAREN: printtoken(line_num, "TPAREN", yytext, -1); break;             // (, )
        case TLBRACE: printtoken(line_num, "TLBRACE", yytext, -1); break;           // {
        case TRBRACE: printtoken(line_num, "TRBRACE", yytext, -1); break;           // }
        case TSEMICOLON: printtoken(line_num, "TSEMICOLON", yytext, -1); break;     // ;
        case TCOMMA: printtoken(line_num, "TCOMMA", yytext, -1); break;             // ,
        case TMAIN: printtoken(line_num, "TMAIN", yytext, -1); break;               // main
        case TPRINT: printtoken(line_num, "TPRINT", yytext, -1); break;             // print, printf
        case TSTRING: printtoken(line_num, "TSTRING", yytext, -1); break;           // string
        case TIDENT: {                                                              // identifier
            //식별자인 경우만 sym_table에 저장. 식별자 저장 후, 그거랑 별개로 매 줄마다 출력하는 printtoken 해줌
            Symtable(line_num, yytext, "TIDENT");
            if (flag != 1)
            {
                //already exist거나 15자 초과면 출력하면 안됨
                printtoken(line_num, "TIDENT", yytext, sym_table_index - 1);  break;
            }
            else { flag = 0; break; }
        }
        case TNUMBER: printtoken(line_num, "TNUMBER", yytext, -1); break;           // number
        case TERROR1: reporterror1(line_num, yytext); break;                        // 숫자로 시작하는 에러 처리
        case TERROR2: reporterror2(line_num, yytext); break;                        //illegal 문자 에러 처리   
        case TERROR3: reporterror3(line_num, yytext); break;                        //나머지 illegal 문자 에러 처리
        }
    }

    printST();
    printHT();
    return 0;
}