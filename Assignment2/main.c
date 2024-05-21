/*
[ COMPILER ASSIGNMENT 02 _ CONTRIBUTION ]

1. 2076362 JAEUN JEON (33%)
2. 2076316 YEEUN LEE (33%)
3. 2176211 JIWON YANG (33%)
*/


/*
 [ COMPILER ASSIGNMENT 02 _ REQUIRED FUNCTIONS ]

 1. �� identifier�� �νĵǸ�, Symbol Table���� identifier�� �����Ѵ�.
    1) identifier�� string_pool ���� �ε����� length�� ����.
    2) printST������ ��ϵ� indentifier�� ���� �ε���, ����, ������ ���.


 2. Source program�� ������ �ִ� ���, ������ ��Ȯ�ϰ� error message�� ����Ʈ�Ѵ�.
    1) �̹� ����� identifier�� ���, Symbol Table�� �������� ����.

    2) 15�� �̻��� identifier�� ���, ���� �޽����� ����ϰ� Symbol Table�� �������� ����.
       : "Error - Identifier length exceeds 15 characters."

    3) String Pool�� ���� �� ���, Overflow �޽����� ����ϰ� �ش� identifier�� Symbol Table�� �������� ����.
       : "Error - String Pool Overflow (The token just below this line is not stored in the string pool)"

    4) ���ڷ� �����ϴ� identifier�� ���, Symbol Table�� �������� ����.
       : "Error at line %d: start with digit."

    5) ������� �ʴ� ���ڸ� �����ϰ� �ִ� ���, Symbol Table�� �������� ����.
        : "Error at line %d: %s is not valid."
        : '������� �ʴ� ����' �� ADDIGIONAL DEFINITIONS���� �����Ѵ�.


 3. �� ��ū�� �νĵǾ��� ��, ��ū�� ������ ����Ʈ�Ѵ�.
    0) printtoken()�� Source program�� ���Ե� ��ū�� ��� �ν��Ͽ� ����Ѵ�. (overflow �߻� ����, symbol table ���� ���� ����)
    1) �ĺ����� ��� : line number, token type, token, ST_index ���
    2) �ĺ��ڰ� �ƴ� ��� : line number, token type, token ���
 */


 /*
  [ COMPILER ASSIGNMENT 02 _ ADDITIONAL DEFINITIONS ]

  1. ����� (keyword) ����
    : "const", "else", "if", "int", "float", "char", "string", "return", "void", "while", "main", "print", "printf"

  2. ������ (operator) ����
   2-1. ��Ģ ������: "+", "-", "*", "/", "%"
   2-2. ���� ������: "=", "+=", "-=", "*=", "/=", "%="
   2-3. �� ������: "!", "&&", "||"
   2-4. ���� ������: "==", "!=", "<", ">", "<=", ">="
   2-5. ���� ������: "++", "--"

  3. Special Symbols ����
    : "(", ")", ".", ",", "{", "}", "[", "]", ";", \"

  4. illegal character ����
   4-1. ������ ���� ���ڴ� `~@#$^ �� �����Ѵ�.
   4-2. ������� �ʴ� ���ڰ� ���Ե� ���ڿ��� �ش� ���ڿ� ��ü�� ������ ó���Ѵ�.
   4-3. �����ڿ� �ش��ϴ� ���ڴ� ���� ó������ �ʴ´�. ������ ��߳��� ǥ���� ���� �ǹ̺м��� ���� �� ������ ����

  5. ����(�Ǽ���) ó�� - TFLOATNUM
    5-1. �����Ҽ����� ����ϴ� ���� ������� �ʴ´�.
    5-2. �ε��Ҽ����� ����ϴ� ���, ���ڷ� �ν��Ѵ�.

  6. ����(������) ó�� - TINTNUM
    6-1. ������ ���ڴ� 10����, 8����, 16���� ���¸� �ν��Ѵ�.

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
        // ������� ��� (�ɺ� �ε����� -1)
        printf("%-16d %-16s %-16s\n", line_num, token_type, token_name);
    }
    else {
        // �ĺ����� ��� (�ɺ� �ε����� ��ȿ�� ���)
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
            //�ĺ����� ��츸 sym_table�� ����. �ĺ��� ���� ��, �װŶ� ������ �� �ٸ��� ����ϴ� printtoken ����
            Symtable(line_num, yytext, "TIDENT");
            //�ű� �ĺ����� ���
            if (flag == 0)
            {
                printtoken(line_num, "TIDENT", yytext, sym_table_index - 1);  break;
            }
            //overflow�� ���
            else if (flag == 2) {
                printtoken(line_num, "TIDENT", yytext, -1); break;
            }
            //�̹� �����ϴ� �ĺ����� ���
            else if (flag == 3) {
                printtoken(line_num, "TIDENT", yytext, temp_index); break;
            }
            //15�� �ʰ��� ���
            else { flag = 0; break; }
        }
        case TINTNUM: printtoken(line_num, "TINTNUM", yytext, -1); break;           
        case TFLOATNUM: printtoken(line_num, "TFLOATNUM", yytext, -1); break;

        case TERROR1: reporterror1(line_num, yytext); break;                        // ���ڷ� �����ϴ� ���� ó��
        case TERROR2: reporterror2(line_num, yytext); break;                        //illegal ���� ���� ó��   
        case TERROR3: reporterror3(line_num, yytext); break;                        //������ illegal ���� ���� ó��
        }
    }

    printST();
    printHT();
    return 0;
}