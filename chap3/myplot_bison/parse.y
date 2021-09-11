%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "obj0-lib.h"
int getch();
void ungetch();
void yyerror(char *);
int  yylex();
struct object *parse_result;
%}

%start start

%union {
	int ival;
	struct object *obj;
	struct list   *array;
}

%token <ival> T_NUMBER
%type <obj> integer
%type <obj> list
%type <array> array

%left ','

%%
start : list ';' {parse_result = (struct object *)$1; }

list    : '[' array ']' { $$ = new_object_list($2); }
        ;
array   : array ',' array { $$ = concat_list($1,$3); }
        | integer { $$ = new_list($1); }
        | list { $$ = new_list($1); }
        ;

integer : T_NUMBER { $$=new_object_mint($1); }
        | '-' T_NUMBER {$$=new_object_mint(-(int)$2); }
        ;

%%
void yyerror(char *s)
{
	fprintf(stderr,"parse.y: %s\n",s);
}

int yylex()
{
	int val=0, c=getch();
	while(isspace(c)) {
		c=getch();
	}
	if(c==EOF) {
		return 0;
	}else if (isdigit(c)) {
		do {
			val=10*val+(c-'0');
			c=getch();
		}while(isdigit(c));
		ungetch();
		yylval.ival = val;
		return T_NUMBER;
	}
	return c;
}
