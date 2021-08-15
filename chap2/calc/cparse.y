%{
#define malloc(x) GC_malloc(x)
#define free(x) GC_free(x)

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "calc.h"
void yyerror(char *);
Poly result;
Poly pvar[26];
%}

%start start

%union {
  Poly f;
  char *s;
  int i;
  pointer p;
}

%token <s> VAR
%token <s> INT
%token <i> PVAR
%token <i> '+' '-' '*' '/' '^' '[' ']' '='

%type <f> expr

%right '='
%left '+' '-' 
%left PLUS
%left MINUS
%left '*' '/'
%right '^'

%%

start : expr ';'
  { 
    result = $1; 
    YYACCEPT; 
  }
;
expr : '+' expr   %prec PLUS
  { $$ = $2; }
     | '-' expr   %prec MINUS
  { $$ = neg_poly($2); }
     | expr '+' expr
  { $$ = add_poly($1,$3); }
     | expr '-' expr
  { $$ = sub_poly($1,$3); }
     | expr '*' expr
  { $$ = mul_poly($1,$3); }
     | expr '^' INT
  { $$ = power_poly($1,$3); }
     | INT
  { $$ = itop($1); }
     | VAR
  { $$ = vtop($1); }
     | PVAR
  { $$ = pvar[$1]; }
     | PVAR '=' expr
  { pvar[$1] = $3; $$ = $3; }
     | '(' expr ')'
  { $$ = $2; }
;
%%

void yyerror(char *s)
{
  fprintf(stderr,"parser : %s\n",s);
}

int yylex()
{
  int c,i,bufsize;
  char *buf,*s;;

  buf = GC_malloc(BUFSIZ);
  bufsize = BUFSIZ;

  switch ( c = skipspace() ) {
    case EOF :  
     exit(0);
      break;
    case '+': case '-': case '*': case '/': case '^':
    case '(': case ')': case ';': case ',': case '=':
     yylval.i = c;
     return c;
     break;
    default:
     break;
  }
  if ( isdigit(c) ) {
    buf[0] = c;
    for ( i = 1; ; i++ ) {
      if ( i == bufsize ) {
        bufsize *= 2;
        buf = GC_realloc(buf,bufsize);
      }
      c = getc(Input);
      if ( !isdigit(c) ) {
        ungetc(c,Input);
        buf[i] = 0;
        break;
      } else
        buf[i] = c;
    }
    s = (char *)GC_malloc(i+1);
    strcpy(s,buf);
    yylval.s = s;
    return INT;
  } else if ( islower(c) ) {
    buf[0] = c;
    for ( i = 1; ; i++ ) {
      c = getc(Input);
      if ( !isalnum(c) ) {
        ungetc(c,Input);
        buf[i] = 0;
        break;
      } else
        buf[i] = c;
    }
    s = GC_malloc(i+1);
    strcpy(s,buf);
    yylval.s = s;
    return VAR;
  } else if ( isupper(c) ) {
    yylval.i = c-'A';
    return PVAR;
  } else
   return 0; // dummy return
}
