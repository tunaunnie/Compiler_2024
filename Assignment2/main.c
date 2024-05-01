#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void initST() {
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        sym_table[i][0] = -1;
        sym_table[i][1] = NULL;
        sym_table[i][2] = NULL;
        sym_table[i][3] = NULL;
    }
}

void printST() {
    printf("\n\n\nSymbol Table:\n");
    int i;
    printf("\[first line #] \t [type] \t [token] \t [length]\n");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1)
            printf("%d\t\t%s\t\t%s\t\t%d\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][2], sym_table[i][2]);
    }
}

void printtoken(int line_num, char* token_type, char* token_name, int sym_index_num) {

    if (sym_index_num == -1)    //예약어인 경우
        printf("%d\t\t%s\t\t%s\n", line_num, token_type, token_name);
    else                        //식별자인 경우
        printf("%d\t\t%s\t\t%s\t\t%d\n", line_num, token_type, token_name, sym_index_num);
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
        case TCONST: printtoken(line_num, "TCONST", yytext, -1); break;                  //main.c 갔다가 symtable.c 갔다가 가능.
        case TELSE: printtoken(line_num, "TELSE", yytext, -1); break;
        case TIF: printtoken(line_num, "TIF", yytext, -1); break;
        case TINT: printtoken(line_num, "TINT", yytext, -1); break;
        case TRETURN: printtoken(line_num, "TRETURN", yytext, -1); break;
        case TVOID: printtoken(line_num, "TVOID", yytext, -1); break;
        case TWHILE: printtoken(line_num, "TWHILE", yytext, -1); break;
        case TEQUAL: printtoken(line_num, "TEQUAL", yytext, -1); break;
        case TNOTEQU: printtoken(line_num, "TNOTEQU", yytext, -1); break;
        case TLESS: printtoken(line_num, "TLESS", yytext, -1); break;
        case TGREAT: printtoken(line_num, "TGREAT", yytext, -1); break;
        case TAND: printtoken(line_num, "TAND", yytext, -1); break;
        case TOR: printtoken(line_num, "TOR", yytext, -1); break;
        case TINC: printtoken(line_num, "TINC", yytext, -1); break;
        case TDEC: printtoken(line_num, "TDEC", yytext, -1); break;
        case TADDASSIGN: printtoken(line_num, "TADDASSIGN", yytext, -1); break;
        case TSUBASSIGN: printtoken(line_num, "TSUBASSIGN", yytext, -1); break;
        case TMULASSIGN: printtoken(line_num, "TMULASSIGN", yytext, -1); break;
        case TDIVASSIGN: printtoken(line_num, "TDIVASSIGN", yytext, -1); break;
        case TMODASSIGN: printtoken(line_num, "TMODASSIGN", yytext, -1); break;
        case TPLUS: printtoken(line_num, "TPLUS", yytext, -1); break;
        case TMINUS: printtoken(line_num, "TMINUS", yytext, -1); break;
        case TSTAR: printtoken(line_num, "TSTAR", yytext, -1); break;
        case TSLASH: printtoken(line_num, "TSLASH", yytext, -1); break;
        case TMOD: printtoken(line_num, "TMOD", yytext, -1); break;
        case TASSIGN: printtoken(line_num, "TASSIGN", yytext, -1); break;
        case TIDENT: {                                                                //식별자인 경우만 sym_table에 저장. 식별자 저장 후, 그거랑 별개로 매 줄마다 출력하는 printtoken 해줌
            Symtable(line_num, yytext, "TIDENT");
            if (flag != 1)
            {
                printtoken(line_num, "TIDENT", yytext, sym_table_index - 1);  break;   //already exist거나 15자 초과면 출력하면 안되니까..
            }
            else { flag = 0; break; }
        }
        case TNUMBER: printtoken(line_num, "TNUMBER", yytext, -1); break; //숫자로 시
        case TERROR: //허용하지 않은 문자를 포함하고 있는 경우
            reporterror(line_num, yytext, "TERROR");
            break;
        }
    }

    printST();
    printHT();
    return 0;
}