%option noyywrap
%option bison-bridge
%option noinput nounput

%{
#include "../wam/include/main.h"
#include "../parser/cwam_parser.h"

#include <math.h>
#include <string.h>

extern SymbolTable *s_table;
extern char file_name[100];

int c_col = 0;
int error_count = 0;
int error_col = 0;
char error[200];

void yyerror (char const *);

void check_error(){
        if (error_count > 0){
                printf("\nERROR: %s:%d:%d: Lexical error: Unrecognized Token Found: %s\n", file_name, yylineno, error_col, error);
        }
        error_count = 0;
}
%}

%option yylineno

DIGIT    [0-9]
INT "-"?{DIGIT}+
FLOAT "-"?{DIGIT}+"."{DIGIT}*
CONSTANTE [a-z][A-Za-z0-9]*(_[A-Za-z0-9]+)*
VAR [A-Z][A-Za-z0-9]*(_[A-Za-z0-9]+)*
SINGLE_VAR "_"+[A-Za-z0-9]*(_[A-Za-z0-9]+)*
ANON_VAR "_"

%%

\n {check_error(); c_col = 0;}

{INT} {check_error(); c_col += yyleng;  strcpy(yylval->term, yytext); return CON;}

{FLOAT}  {check_error(); c_col += yyleng; strcpy(yylval->term, yytext); return CON;}

{CONSTANTE}  {check_error(); c_col += yyleng; strcpy(yylval->term, yytext); return CON;}

{VAR} {check_error(); c_col += yyleng; strcpy(yylval->term, yytext); return VAR;}

{ANON_VAR} {check_error(); c_col += yyleng; strcpy(yylval->term, yytext); return ANON_VAR;}

{SINGLE_VAR} {check_error(); c_col += yyleng; strcpy(yylval->term, yytext); return SINGLE_VAR;}


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
                        c_col = 0;
                }

"%"[^/\n]* { check_error(); c_col=0;}

[ \t]+ {check_error(); c_col += yyleng;}

.  {
        c_col += 1;
        if (error_count == 0){
                error_col = c_col;
        }
        error[error_count] = yytext[0];
        error_count += 1;
   }

%%

void yyerror ( char const *s) {
    printf("ERROR: %s:%d:%d:", file_name, yylineno, error_col);
	printf("%s", s);
}


