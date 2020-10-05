%debug
%defines
%define parse.error verbose
%define api.pure

%{
	#include <stdio.h>
	#include <ctype.h>

	#define YYSTYPE double

	void yyerror (char const *);
    int yylex();
%}

%token CON
%token VAR
%token LIS
%token RULE_SYM

%%

programa:
     predicado
     | programa predicado
;

predicado:
    clausula
    | predicado clausula
;

clausula:
    fato
    | regra
;

fato:
    estrutura '.'
;

regra:
    estrutura RULE_SYM estruturas '.'
;

estruturas:
    estrutura
    | estrutura ',' estruturas '.'
;

estrutura:
    CON
    | CON '(' argumentos ')'
;

argumentos:
    termo
    | termo ',' argumentos
;

termo:
    estrutura
    | VAR
    | LIS
;

%%

void yyerror (char const *s) {
	fprintf (stderr, "%s\n", s);
}

int main (void) {
	return yyparse();
}

// int main( int argc, char **argv )
//     {
//     FILE* fp;
//     ++argv, --argc;  /* skip over program name */
//     if ( argc > 0 ) {
// 	    fp = fopen(argv[0], "r");
//             yyin = fp;
//             strcpy(file_name, argv[0]);
//     } else {
// 	    return 0;
//     }
//     yylex();
//     yylex_destroy();
//     fclose(fp);
//     return 0;
// }
