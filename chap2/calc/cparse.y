%{
#if defined(USEGC)
#define malloc(x) MALLOC(x)
#define free(x) FREE(x)
#endif

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "calc.h"
void yyerror(char *);
Poly result;
Poly pvar[26];
FILE *Input;
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
     | expr '/' expr
  { $$ = divc_poly($1,$3); }
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

extern char *parse_string;
extern int parse_string_index;

int Getc()
{
  if ( parse_string != 0 ) {
    return parse_string[parse_string_index++];
  } else
    return getc(Input);
}

void Ungetc(int c)
{
  if ( parse_string != 0 ) {
    parse_string[--parse_string_index] = c;
  } else
    ungetc(c,Input);
}

int yylex()
{
  int c,i,bufsize;
  char *buf,*s;;

  buf = MALLOC(BUFSIZ);
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
        buf = REALLOC(buf,bufsize);
      }
      c = Getc();
      if ( !isdigit(c) ) {
        Ungetc(c);
        buf[i] = 0;
        break;
      } else
        buf[i] = c;
    }
    s = (char *)MALLOC(i+1);
    strcpy(s,buf);
    yylval.s = s;
    return INT;
  } else if ( islower(c) ) {
    buf[0] = c;
    for ( i = 1; ; i++ ) {
      c = Getc();
      if ( !isalnum(c) ) {
        Ungetc(c);
        buf[i] = 0;
        break;
      } else
        buf[i] = c;
    }
    s = MALLOC(i+1);
    strcpy(s,buf);
    yylval.s = s;
    return VAR;
  } else if ( isupper(c) ) {
    yylval.i = c-'A';
    return PVAR;
  } else
   return 0; // dummy return
}
