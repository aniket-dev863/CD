%{
#include<stdio.h>
%}
%option noyywrap

VALID_ID [a-zA-Z_][a-zA-Z0-9_]*
INVALID_ID_NUM [0-9][a-zA-Z0-9_]+

%%
{VALID_ID} {printf("'%s' -> Valid Indetifer \n", yytext);}

{INVALID_ID_NUM} {printf(" '%s' -> Invalid Identiefer (Cannot Start with Number )\n",yytext);}

[0-9]+

[ \t\n]+

.

%%

int main(){
    printf("Enter the Text: ");
    yylex();
    printf("Exiting .....\n");
    return 0 ;
}