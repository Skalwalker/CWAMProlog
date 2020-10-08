#include "../include/main.h"

extern FILE *yyin;

char file_name[100];

SymbolTable *s_table = NULL;

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

    yyparse();
    yylex_destroy();
    fclose(fp);
    st_print();
    st_delete();
	return 0;
}
