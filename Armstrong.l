%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int isArm(char*string){
    int n = strlen(string);
    if (n==0)return 0 ; 
    long long num =atoll(string);
    long long temp =num ;
    long long sum=0;
    while(temp>0){
        long long digit = temp%10;
        long long p=1;
        for(int i=0;i<n;i++){
            p*=digit;
        }
        sum+=p;
        temp/=10;
    }
    if(sum==num){
        return 1;
    }
    else{
        return 0 ;
    }
}
%}
%option noyywrap

%%
[0-9]+  {
            if(isArm(yytext)){
                printf("%s is a ArmStrong Number \n",yytext);
            }
            else{
                printf("%s is Not a Armstrong Number \n",yytext);
            }
        }

\n      {printf("\n");}

.

%%

int main(){
    printf("Enter the number to check : ");
    yylex();
    printf("..Exiting..\n");
    return 0;
}