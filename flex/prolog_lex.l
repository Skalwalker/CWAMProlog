/* scanner for a toy Pascal-like language */
%option noinput nounput

%{
/* need this for the call to atof() below */
#include <math.h>
#include <string.h>

int c_line = 1;
int c_col = 0;
int error_count = 0;
int error_line = 0;
int error_col = 0;
char file_name[100];
char error[100];


void check_error(){
        if (error_count > 0){
                printf("\nERROR: %s:%d:%d: Lexical error: Unrecognized Token Found: %s\n", file_name, error_line, error_col, error);
        }
        error_count = 0;
}
%}

DIGIT    [0-9]
INT "-"?{DIGIT}+
FLOAT "-"?{DIGIT}+"."{DIGIT}*
CONSTANTE [a-z][A-Za-z0-9]*(_[A-Za-z0-9]+)*
VARIAVEL [_A-Z][A-Za-z0-9]*(_[A-Za-z0-9]+)*

%%

\n {check_error(); c_col = 0; c_line++;}

{INT} {check_error(); c_col += yyleng; printf("<CON, %d>", atoi( yytext ));}

{FLOAT}  {check_error(); c_col += yyleng; printf("<CON, %d>", atoi( yytext ));}

{CONSTANTE}  {check_error(); c_col += yyleng; printf("<CON, %s>", yytext);}

{VARIAVEL} {check_error(); c_col += yyleng; printf("<REF, %s>", yytext);}

"(" {check_error(); c_col += 1; printf("<(>");}

")" {check_error(); c_col += 1; printf("<)>");}

"." {check_error(); c_col += 1; printf("<.>\n");}

"|" {check_error(); c_col += 1; printf("<|>");}

"," {check_error(); c_col += 1; printf("<,>");}

":-" {check_error(); c_col += 2; printf("<:->");}

"[" {check_error(); c_col += 1; printf("<[>");}

"]" {check_error(); c_col += 1; printf("<]>");}

"/*"[^/]*"*/"  {
                        check_error();
                        for (int i=0; i < yyleng; i++){
                                if (strncmp(&yytext[i], "\n", 1  ) == 0) {
                                        c_line += 1;
                                }
                        }
                        c_col = 0;
                }


[ \t]+ {check_error(); c_col += yyleng;}

.  {
        c_col += 1;
        if (error_count == 0){
                error_col = c_col;
                error_line = c_line;
        }
        error[error_count] = yytext[0];
        error_count += 1;
   }

%%

int main( int argc, char **argv )
    {
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 ) {
            yyin = fopen( argv[0], "r" );
            strcpy(file_name, argv[0]);
    } else {
            yyin = stdin;
    }
    yylex();
    yylex_destroy();
    return 0;
}
