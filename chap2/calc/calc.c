#include "calc.h"
#include <time.h>

Ring CurrentRing;
FILE *Input;
extern Poly result;

void *GC_calloc_atomic(size_t n)
{
  void *p;
  p = GC_malloc_atomic(n);
  memset(p,0,n);
  return p;
}

void error(char *msg)
{
  fprintf(stderr,"%s\n",msg);
  exit(0);
}

// exp should be placed in the reverse order.
// (e1 ... en) -> (en ... e1)

int mcomp_simple(Monomial a,Monomial b)
{
  int i,w,ret;

  if ( CurrentRing->graded ) {
    if ( a->td > b->td ) return 1;
    else if ( a->td < b->td ) return -1;
  }
  w = CurrentRing->wpe;
  ret = CurrentRing->rev?-1:1;
  for ( i = 0; i < w; i++ )
    if ( a->exp[i] > b->exp[i] ) return ret;
    else if ( a->exp[i] < b->exp[i] ) return -ret;
  return 0;
}

int length(Node p)
{
  int i;
  for ( i = 0; p; p = p->next, i++ );
  return i;
}

int ishomo_poly(Poly p)
{
  LONG td;
  Poly q;

  if ( p == 0 ) return 1;
  td = p->m->td;
  for ( q = p->next; q != 0; q = q->next )
    if ( q->m->td != td ) return 0;
  return 1;
}

LONG tdeg_poly(Poly p)
{
  LONG td;
  Poly q;

  if ( p == 0 ) return -1;
  td = 0;
  for ( q = p; q != 0; q = q->next )
    if ( q->m->td > td ) td = q->m->td;
  return td;
}

Poly neg_poly(Poly p)
{
  struct poly root;
  Poly q,r,s;

  if ( p == 0 ) return p;
  r = &root;
  for ( q = p; q != 0; q = q->next ) {
     APPENDPOLY(r,s,CurrentRing->negc(q->c),q->m);
  }
  r->next = 0;
  return root.next;
}

Poly add_poly(Poly p1,Poly p2)
{
  struct poly root;
  Poly r,q1,q2,q;
  Coef c;
  int (*mcomp)(Monomial,Monomial);

  if ( p1 == 0 )
    return p2;
  else if ( p2 == 0 )
    return p1;
  else {
    r = &root;
    mcomp = CurrentRing->mcomp;
    for ( q1 = p1, q2 = p2; q1 != 0 && q2 != 0; ) {
      switch ( (*mcomp)(q1->m,q2->m) ) { 
        case 0:
          c = CurrentRing->addc(q1->c,q2->c);
          if ( c.f != 0 ) {
            APPENDPOLY(r,q,c,q1->m);
          }
          q1 = q1->next; q2 = q2->next;
          break;
        case 1:
          APPENDPOLY(r,q,q1->c,q1->m);
          q1 = q1->next;
          break;
        case -1:
          APPENDPOLY(r,q,q2->c,q2->m);
          q2 = q2->next;
          break;
      }
    }
    if ( q1 ) {
      r->next = q1;
    } else {
      r->next = q2;
    }
    return root.next;
  }
}

Poly merge_poly(Poly p1,Poly p2)
{
  struct poly root;
  Poly r,q1,q2,q,t;
  Coef c;
  int (*mcomp)(Monomial,Monomial);

  if ( p1 == 0 )
    return p2;
  else if ( p2 == 0 )
    return p1;
  else {
    r = &root;
    mcomp = CurrentRing->mcomp;
    for ( q1 = p1, q2 = p2; q1 != 0 && q2 != 0; ) {
      switch ( (*mcomp)(q1->m,q2->m) ) { 
        case 0:
          c = CurrentRing->addc(q1->c,q2->c);
          if ( c.f != 0 ) {
            q1->c = c; r->next = q1; r = q1; q1 = q1->next; 
          } else {
            t = q1->next; GC_free(q1->m); GC_free(q1); q1 = t;
          }
          t = q2->next; GC_free(q2->m); GC_free(q2); q2 = t;
          break;
        case 1:
          r->next = q1; r = q1;
          q1 = q1->next;
          break;
        case -1:
          r->next = q2; r = q2;
          q2 = q2->next;
          break;
      }
    }
    if ( q1 ) {
      r->next = q1;
    } else {
      r->next = q2;
    }
    return root.next;
  }
}

Poly sub_poly(Poly p1,Poly p2)
{
  return add_poly(p1,neg_poly(p2));
}

Poly mulc_poly(Coef c,Poly p)
{
  struct poly root;
  Poly q,r,s;

  r = &root;
  for ( q = p; q != 0; q = q->next ) {
     APPENDPOLY(r,s,CurrentRing->mulc(c,q->c),q->m);
  }
  r->next = 0;
  return root.next;
}

Monomial mul_monomial(Monomial m1,Monomial m2)
{
  Monomial m;
  int w,i;

  NEWMONOMIAL(m);
  w = CurrentRing->wpe;
  m->td = m1->td+m2->td;
  for ( i = 0; i < w; i++ ) m->exp[i] = m1->exp[i]+m2->exp[i];
  return m;
}

Poly mul1_poly(Poly p1,Poly p2)
{
  struct poly root;
  Poly p,q,r,s;
  Coef c,c1;
  Monomial m;

  r = &root;
  c = p1->c;
  for ( q = p2; q != 0; q = q->next ) {
     c1 = CurrentRing->mulc(c,q->c); 
     m = mul_monomial(p1->m,q->m);
     APPENDPOLY(r,s,c1,m);
  }
  r->next = 0;
  return root.next;
}

Poly mul_poly(Poly p1,Poly p2)
{
  Poly r,p,q;

  r = 0;
  for ( q = p1; q != 0; q = q->next ) {
    p = mul1_poly(q,p2);
    r = merge_poly(r,p);
  }
  return r;
}

Poly power_poly(Poly p,char *q)
{
  Poly r,pi;
  int e;

  e = strtol(q,0,10);
  if ( e < 0 )
    error("power_poly : exponent must be non-negative");
  // e = sum ei*2^i => p^e = prod_{ei=1} p^(2^i)
  // pi <- p^(2^0)=p; r <- 1
  NEWPOLY(r);
  NEWMONOMIAL(r->m);
  r->c = CurrentRing->one;
  pi = p;
  while ( e != 0 ) {
    if ( (e&1) != 0 )
      r = mul_poly(r,pi);
    e >>= 1;
    if ( e != 0 )
       pi = mul_poly(pi,pi);
  }
  return r;
}

Poly itop(char *n)
{
  Poly r;
  Monomial m;

  NEWPOLY(r);
  NEWMONOMIAL(m);
  m->td = 0;
  r->m = m;
  r->c = CurrentRing->ntoc(n);
  return r;
}

Poly vtop(char *v)
{
  int nv,i,wpos,bpos,shift;
  char **vname;
  Poly r;
  Monomial m;

  nv = CurrentRing->nv;
  vname = CurrentRing->vname;

  for ( i = 0; i < nv; i++ )
    if ( !strcmp(v,vname[i]) ) break;
  if ( i == nv ) return 0;
  if ( CurrentRing->rev ) i = nv-1-i;
  NEWPOLY(r);
  NEWMONOMIAL(m);
  m->td = 1;
  bpos = i*CurrentRing->bpe;
  wpos = bpos/8; shift = 8-CurrentRing->bpe-(bpos%8);
  m->exp[wpos] = (1UL)<<(shift*8);
  r->c = CurrentRing->one;
  r->m = m;
  r->next = 0;
  return r;
}

// bpe = byte per an exponent; 1,2,4,8
Ring create_ring(Node vars,int type,int bpe,ULONG chr)
{
  Ring r;
  int i;
  Node p;

  NEWRING(r);
  r->nv = length(vars);
  r->vname = (char **)GC_malloc(r->nv*sizeof(char *));
  for ( p = vars, i = 0; p; p = p->next, i++ )
    r->vname[i] = (char *)p->body;
  r->bpe = bpe;
  r->wpe = (r->nv*bpe+7)/8;
  switch ( type ) {
    case 0: // grevlex
      r->graded = 1;
      r->rev = 1;
      r->mcomp = mcomp_simple;
      break;
    case 1: // glex
      r->graded = 1;
      r->rev = 0;
      r->mcomp = mcomp_simple;
      break;
    case 2: // lex
      r->graded = 0;
      r->rev = 0;
      r->mcomp = mcomp_simple;
      break;
    default:
      r = 0;
      break;
  }
  r->chr = chr;
  switch ( bpe ) {
    case 1:
     r->sb = SB1; break;
    case 2:
     r->sb = SB2; break;
    case 4:
     r->sb = SB4; break;
    case 8:
     r->sb = SB8; break;
    default:
     r = 0; break;
  }
  if ( chr == 0 ) {
    r->ntoc = ntoc_z;
    r->addc = add_z;
    r->subc = sub_z;
    r->mulc = mul_z;
    r->divc = 0;
    r->negc = neg_z;
    r->printc = print_z;
    r->one = one_z();
  } else if ( chr == 1 ) {
    r->ntoc = ntoc_q;
    r->addc = add_q;
    r->subc = sub_q;
    r->mulc = mul_q;
    r->divc = div_q;
    r->negc = neg_q;
    r->printc = print_q;
    r->one = one_q();
  } else {
    r->ntoc = ntoc_ff;
    r->addc = add_ff;
    r->subc = sub_ff;
    r->mulc = mul_ff;
    r->divc = div_ff;
    r->negc = neg_ff;
    r->printc = print_ff;
    r->one = one_ff();
  }
  return r;      
}

FILE *Input;

int skipspace() {
  int c;

  for ( c = getc(Input); ; )
    switch ( c ) {
      case ' ': case '\t': case '\r': case '\n':
        c = getc(Input);
        break;
      default:
        return c;
         break;
    }
}

void print_monomial(Monomial m)
{
  ULONG mask,e;
  int nv,bpe,i,shift,bpos,wpos;

  nv = CurrentRing->nv;
  bpe = CurrentRing->bpe;
  if ( bpe == 8 ) mask = ~0;
  else mask = ((1UL)<<(bpe*8))-1;
  printf("<<");
  if ( CurrentRing->rev ) {
    for ( i = nv-1; i >= 0; i-- ) {
      bpos = i*bpe; wpos = bpos/8; shift = 8-bpe-(bpos%8);
      e = (m->exp[wpos] >> (shift*8))&mask;
      printf("%lld",e);
      if ( i != 0 ) putchar(',');
    }
  } else {
    for ( i = 0; i < nv; i++ ) {
      bpos = i*bpe; wpos = bpos/8; shift = 8-bpe-(bpos%8);
      e = (m->exp[wpos] >> (shift*8))&mask;
      printf("%lld",e);
      if ( i != nv-1 ) putchar(',');
    }
  }
  printf(">>");
}

void print_poly(Poly p)
{
  Poly q;

  for ( q = p; q != 0; q = q->next ) {
    putchar('+');
    CurrentRing->printc(q->c);
    putchar('*');
    print_monomial(q->m);
  }
}

Node get_vars()
{
  struct node root;
  Node p,p1;
  int c,i;
  char buf[BUFSIZ];
  char *s;

  p = &root;
  while ( (c = getc(Input)) != '[' );
  while ( 1 ) {
    c = skipspace();
    if ( c == ']' ) {
      p->next = 0;
      return root.next;
    } else {
      buf[0] = c;
      for ( i = 1; ; i++ ) {
        if ( i == BUFSIZ )
          error("get_vars : variable name too long");
        c = getc(Input);
        if ( !isalnum(c) ) {
          ungetc(c,Input);
          buf[i] = 0;
          break;
        } else
          buf[i] = c;
      }
      s = (char *)GC_malloc(i+1);
      strcpy(s,buf);
      APPENDNODE(p,p1,s);
    }
  }
}

void print_node(Node p)
{
  Node q;

  for ( q = p; q != 0; q = q->next ) {
    print_poly((Poly)q->body); printf("\n");
  }
}

void *gc_realloc(void *p,size_t osize,size_t nsize)
{
  return (void *)GC_realloc(p,nsize);
}

// input file format :
// chr ordid bpe
// [x y z ...]
// p1,p2,...,pn;

int main(int argc,char **argv)
{
  Node vars,out;
  int chr,ordid,bpe,alg;
  
  if ( argc < 2 ) {
    fprintf(stderr,"usage : %s file [infile]\n",argv[0]);
    exit(0);
  }
  Input = fopen(argv[1],"r"); 
  if ( Input == 0 ) {
    fprintf(stderr,"%s not found\n",argv[1]);
    exit(0);
  }
  GC_init();
  mp_set_memory_functions(
    (void *(*)(size_t))GC_malloc,
    (void *(*)(void *,size_t,size_t))gc_realloc,
    (void (*)(void *,size_t))GC_free);
  fscanf(Input,"%d %d %d\n",&chr,&ordid,&bpe);
  vars = get_vars();
  CurrentRing = create_ring(vars,ordid,bpe,chr);
  if ( argc == 3 )
    Input = fopen(argv[2],"r"); 
  else
    Input = stdin;
  while ( 1 ) {
    yyparse();
    print_poly(result); printf("\n");
  }
}
