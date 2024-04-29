
void init_sym_table() {
    int i;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        sym_table[i][0] = -1;
        sym_table[i][1] = -1;
    }
}

//여기서 line #, token type, toekn 이름, ST index (명칭인 경우만) 출력. 
void print_sym_table() {
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

void print_hash_table() {
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
   init_sym_table();       //여기 들어가는 게 맞나 ..

    while ((tn = yylex()) != TEOF) {
        switch(tn) {
         case LINEPROCESS: line_num++; break;
          case TCONST: symtable(line_num, yytext, "TCONST"); break;                  //main.c 갔다가 symtable.c 갔다가 가능.
          case TELSE : symtable(line_num, yytext, "TELSE"); break;
          case TIF : symtable(line_num, yytext, "TIF"); break;
          case TINT : symtable(line_num, yytext, "TINT"); break;
          case TRETURN : symtable(line_num, yytext, "TRETURN"); break;
          case TVOID : symtable(line_num, yytext, "TVOID"); break;
          case TWHILE : symtable(line_num, yytext, "TWHILE"); break;
          case TEQUAL : symtable(line_num, yytext, "TEQUAL"); break;
          case TNOTEQU : symtable(line_num, yytext, "TNOTEQU"); break;
          case TLESS : symtable(line_num, yytext, "TLESS"); break;
          case TGREAT : symtable(line_num, yytext, "TGREAT"); break;
          case TAND : symtable(line_num, yytext, "TAND"); break;
          case TOR : symtable(line_num, yytext, "TOR"); break;
          case TINC : symtable(line_num, yytext, "TINC"); break;
          case TDEC : symtable(line_num, yytext, "TDEC"); break;
          case TADDASSIGN : symtable(line_num, yytext, "TADDASSIGN"); break;
          case TSUBASSIGN : symtable(line_num, yytext, "TSUBASSIGN"); break;
          case TMULASSIGN : symtable(line_num, yytext, "TMULTASSIGN"); break;
          case TDIVASSIGN : symtable(line_num, yytext, "TDIVASSIGN"); break;
          case TMODASSIGN : symtable(line_num, yytext, "TMODASSIGN"); break;
          case TPLUS : symtable(line_num, yytext, "TPLUS"); break;
          case TMINUS : symtable(line_num, yytext, "TMINUS"); break;
          case TSTAR : symtable(line_num, yytext, "TSTAR"); break;
          case TSLASH : symtable(line_num, yytext, "TSLASH"); break;
          case TMOD : symtable(line_num, yytext, "TMOD"); break;
          case TASSIGN : symtable(line_num, yytext, "TASSIGN"); break;
          case TIDENT : symtable(line_num, yytext, "TIDENT"); break;
          case TNUMBER: symtable(line_num, yytext, "TNUMBER"); break;
          case TERROR: symtable(line_num, yytext, "TERROR"); break;
        }
    }

    print_sym_table();          //여기서 line #, token type, toekn 이름, ST index (명칭인 경우만) 출력. 
    print_hash_table();
    return 0;
}

//한번에 못하나? case문 중첩 이용해서.. 하나씩 break 안때리고.