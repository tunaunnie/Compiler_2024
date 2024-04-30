#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
#include "glob.h"

void initST() {
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        sym_table[i][0] = -1;
        sym_table[i][1] = -1;
    }
}

void printST() {
    int i;
    printf("\nSymbol Table\n");
    printf("\[line #] \t [type] \t [token] \t [ST index] (only TIDENT)\n");
    printf("----------------------------------------------------------------\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1) {
            printf("%d\t\t%s\t\t%s\t\t%d\n", sym_table[i][0], sym_table[i][1], str_pool + sym_table[i][2], sym_table[i][2]);
        }
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

	while ((tn = yylex()) != TEOF) {
		switch (tn) {
        case LINEPROCESS: line_num++; break;
        case TCONST: Symtable(line_num, yytext, "TCONST"); break;                  //main.c °¬´Ù°¡ symtable.c °¬´Ù°¡ °¡´É.
        case TELSE: Symtable(line_num, yytext, "TELSE"); break;
        case TIF: Symtable(line_num, yytext, "TIF"); break;
        case TINT: Symtable(line_num, yytext, "TINT"); break;
        case TRETURN: Symtable(line_num, yytext, "TRETURN"); break;
        case TVOID: Symtable(line_num, yytext, "TVOID"); break;
        case TWHILE: Symtable(line_num, yytext, "TWHILE"); break;
        case TEQUAL: Symtable(line_num, yytext, "TEQUAL"); break;
        case TNOTEQU: Symtable(line_num, yytext, "TNOTEQU"); break;
        case TLESS: Symtable(line_num, yytext, "TLESS"); break;
        case TGREAT: Symtable(line_num, yytext, "TGREAT"); break;
        case TAND: Symtable(line_num, yytext, "TAND"); break;
        case TOR: Symtable(line_num, yytext, "TOR"); break;
        case TINC: Symtable(line_num, yytext, "TINC"); break;
        case TDEC: Symtable(line_num, yytext, "TDEC"); break;
        case TADDASSIGN: Symtable(line_num, yytext, "TADDASSIGN"); break;
        case TSUBASSIGN: Symtable(line_num, yytext, "TSUBASSIGN"); break;
        case TMULASSIGN: Symtable(line_num, yytext, "TMULTASSIGN"); break;
        case TDIVASSIGN: Symtable(line_num, yytext, "TDIVASSIGN"); break;
        case TMODASSIGN: Symtable(line_num, yytext, "TMODASSIGN"); break;
        case TPLUS: Symtable(line_num, yytext, "TPLUS"); break;
        case TMINUS: Symtable(line_num, yytext, "TMINUS"); break;
        case TSTAR: Symtable(line_num, yytext, "TSTAR"); break;
        case TSLASH: Symtable(line_num, yytext, "TSLASH"); break;
        case TMOD: Symtable(line_num, yytext, "TMOD"); break;
        case TASSIGN: Symtable(line_num, yytext, "TASSIGN"); break;
        case TIDENT: Symtable(line_num, yytext, "TIDENT"); break;
        case TNUMBER: Symtable(line_num, yytext, "TNUMBER"); break;
        case TERROR: Symtable(line_num, yytext, "TERROR"); break;
        }
    }

    printST();
    printHT();
    return 0;
}