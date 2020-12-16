#include "../include/main.h"

extern FILE *yyin;
extern int yylex_destroy();
extern int yy_scan_string();

char file_name[100];

SymbolTable *s_table = NULL;


void load_program(){
    FILE* fp;
    fp = fopen(file_name, "r");
    yyin = fp;
    yyparse(1, 0);
    fclose(fp);
    yylex_destroy();
}

void load_query(char *query){
    yy_scan_string(query);
    yyparse(0, 0);
    printf("\n======================== Heap da Query ========================\n");
    print_heap_reversed();
    printf("===============================================================\n");
    yylex_destroy();
}

void process_query(){
    char query[500];
    printf("?- ");
    scanf("%[^\n]s", query);
    getchar();
    load_query(query);
    load_program();
}

int main(int argc, char **argv){
    FILE* fp;
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 ) {
	    fp = fopen(argv[0], "r");
        yyin = fp;
        strcpy(file_name, argv[0]);
    } else {
	    printf("Prolog Rule File Missing! Ending Execution...\n");
		return 0;
    }

    yyparse(-1, 0);
    yylex_destroy();
    fclose(fp);
    st_print();
    st_delete();

    printf("\n\n\n");

    process_query();

	return 0;
}
