%{
int l=0,w=0,c=0;
%}

%option noyywrap

%%
[^ \t\n]+ {w++ ; c+=yyleng;}
\n        {l++;c++;}
.         {c++;}
%%

int main(){
    yylex();
    if(c>0 && l==0){
        l=1;
    }
    printf("\n Printing the counts \n");
    printf(" No of Lines :%d \n",l);
    printf(" No of Characters :%d \n",c);
    printf(" No of Words :%d \n",w);
    return 0 ;
}
