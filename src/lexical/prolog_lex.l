
%option noinput nounput
%option noyywrap
%option bison-bridge

%{
#include "../parser/cwam_parser.h"
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

{INT} {check_error(); c_col += yyleng; return CON;}

{FLOAT}  {check_error(); c_col += yyleng; return CON;}

{CONSTANTE}  {check_error(); c_col += yyleng; return CON;}

{VARIAVEL} {check_error(); c_col += yyleng; return VAR;}

"(" {check_error(); c_col += 1; return '(';}

")" {check_error(); c_col += 1; return ')';}

"." {check_error(); c_col += 1; return '.';}

"|" {check_error(); c_col += 1; return '|';}

"," {check_error(); c_col += 1; return ',';}

":-" {  check_error(); c_col += 2; return RULE_SYM; }

"[" {check_error(); c_col += 1; return '[';}

"]" {check_error(); c_col += 1; return ']';}

"/*"[^/]*"*/"  {
                        check_error();
                        for (unsigned long i=0; i < yyleng; i++){
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


