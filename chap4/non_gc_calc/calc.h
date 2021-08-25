#include <inttypes.h> //追加
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
//#include <gc.h>

typedef int64_t LONG;
typedef uint64_t ULONG;

typedef void *pointer;

typedef struct node {
  void *body;
  struct node *next;
} *Node;

//#define NEWNODE(a) ((a)=(Node)GC_malloc(sizeof(struct node)))
#define NEWNODE(a) ((a)=(Node)calloc(sizeof(struct node),1))

// ...->prev => ...->rev->cur, prev=cur
#define APPENDNODE(prev,cur,b) \
(NEWNODE(cur),(cur)->body=(void *)(b),(prev)->next=(cur),(prev)=(cur))

// prev->... => cur->prev->..., prev=cur
#define CONSNODE(prev,cur,b) \
(NEWNODE(cur),(cur)->body=(void *)(b),(cur)->next=(prev),(prev)=(cur))

typedef struct monomial {
  LONG td;
  ULONG exp[1];
} *Monomial;

#define NEWMONOMIAL(a) ((a)=(Monomial)GC_calloc_atomic(sizeof(struct monomial)+(CurrentRing->wpe-1)*sizeof(ULONG)))

typedef union {
  LONG f;
  mpz_ptr z;
  mpq_ptr q;
} Coef;

//#define NEWZ(a) ((a)=(mpz_ptr)GC_malloc(sizeof(mpz_t)))
//#define NEWQ(a) ((a)=(mpq_ptr)GC_malloc(sizeof(mpq_t)))
//#define NEWCOEF(a) ((a)=(Coef)GC_malloc(sizeof(union coef)))
#define NEWZ(a) ((a)=(mpz_ptr)calloc(sizeof(mpz_t),1))
#define NEWQ(a) ((a)=(mpq_ptr)calloc(sizeof(mpq_t),1))
#define NEWCOEF(a) ((a)=(Coef)calloc(sizeof(union coef),1))

typedef struct poly {
  Coef c;
  Monomial m;
  struct poly *next;
} *Poly;

//#define NEWPOLY(a) ((a)=(Poly)GC_malloc(sizeof(struct poly)))
#define NEWPOLY(a) ((a)=(Poly)calloc(sizeof(struct poly),1))
#define APPENDPOLY(prev,cur,coef,mono) \
(NEWPOLY(cur),(cur)->c=(coef),(cur)->m=(mono),(prev)->next=(cur),(prev)=(cur))

typedef struct ring {
 int nv;
 char **vname;
 int bpe; // byte per an exponent
 int wpe; // number of words per an exponent vector
 int graded; // set to 1 if the order is graded 
 int rev; // set to 1 if the order is revlex
 int (*mcomp)(Monomial,Monomial);
 LONG chr;
 ULONG sb; // sign bits
 Coef (*ntoc)(char *);
 char *(*printc)(Coef); //変更
 Coef (*addc)(Coef,Coef);
 Coef (*subc)(Coef,Coef);
 Coef (*mulc)(Coef,Coef);
 Coef (*divc)(Coef,Coef);
 Coef (*negc)(Coef);
 Coef one;
} *Ring;

extern Ring CurrentRing;
extern FILE *Input;
extern void set_str(char *buf); //追加

Ring create_ring(Node vars,int type,int bpe,ULONG chr);

//#define NEWRING(a) ((a)=(Ring)GC_malloc(sizeof(struct ring)))
#define NEWRING(a) ((a)=(Ring)calloc(sizeof(struct ring),1))

Poly vtop(char *);
Poly itop(char *);
void check(void);
Node append_to_node(Node p,void *obj);
int yyparse(),yylex(),skipspace(),skipspace_str(char *str, int j); //変更
void error(char *);

Poly add_poly(Poly,Poly), sub_poly(Poly,Poly), neg_poly(Poly);
Poly mul_poly(Poly,Poly), divc_poly(Poly,Poly), power_poly(Poly,char *);
Poly dup_poly(Poly), mul1_poly(Poly,Poly);
void free_poly(Poly), print_poly(Poly);
LONG tdeg_poly(Poly p);

Coef one_ff();
char *print_ff(Coef a); //変更
Coef add_ff(Coef a,Coef b), neg_ff(Coef a);
Coef sub_ff(Coef a,Coef b), mul_ff(Coef a,Coef b);
Coef mulsub_ff(Coef c,Coef a,Coef b);
Coef inv_ff(Coef s), div_ff(Coef a,Coef b);
Coef ntoc_ff(char *n);

Coef mpztoc(mpz_ptr t);
Coef one_z();
char *print_z(Coef a); //変更
Coef add_z(Coef a,Coef b), neg_z(Coef a);
Coef sub_z(Coef a,Coef b), mul_z(Coef a,Coef b);
Coef divexact_z(Coef a,Coef b), gcd_z(Coef a,Coef b);
Coef ntoc_z(char *n);

Coef mpqtoc(mpq_ptr t);
Coef one_q();
char *print_q(Coef a); //変更
Coef add_q(Coef a,Coef b), neg_q(Coef a);
Coef sub_q(Coef a,Coef b), mul_q(Coef a,Coef b);
Coef div_q(Coef a,Coef b);
Coef ntoc_q(char *n);

#define SB1 0x8080808080808080
#define SB2 0x8000800080008000
#define SB4 0x8000000080000000
#define SB8 0x8000000000000000
