/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
#include <string.h>

int c_line = 1;
int c_col = 1;
char file_name[100];

%}

DIGIT    [0-9]
INT "-"?{DIGIT}+
FLOAT "-"?{DIGIT}+"."{DIGIT}*
CONSTANTE [a-z][A-Za-z0-9_]*
VARIAVEL (_|[A-Z])[A-Za-z0-9_]*

%%

\n {c_col = 1; c_line++;}

{INT} {c_col += yyleng; printf("<CON, %d>", atoi( yytext ));}

{FLOAT}  {c_col += yyleng; printf("<CON, %d>", atoi( yytext ));}

{CONSTANTE}  {c_col += yyleng; printf("<CON, %s>", yytext);}

{VARIAVEL}  {c_col += yyleng; printf("<REF, %s>", yytext);}

"(" {c_col += 1; printf("<(>");}

")" {c_col += 1; printf("<)>");}

"." {c_col += 1; printf("<.>\n");}

"|" {c_col += 1; printf("<|>");}

"," {c_col += 1; printf("<,>");}

":-" {c_col += 2; printf("<:->");}

"[" {c_col += 1; printf("<[>");}

"]" {c_col += 1; printf("<]>");}

"/*"[^/]*"*/"  /* eat up one-line comments */

[ \t\n]+ {c_col += yyleng;}

.  {    c_col += 1;
        printf("\nERROR: %s:%d:%d: Lexical error: Unrecognized Token Found: %s\n", file_name, c_line, c_col, yytext);
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
}
