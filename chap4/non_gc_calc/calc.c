#include "calc.h"
#include <time.h>

Ring CurrentRing;
FILE *Input;
extern Poly result;

void *GC_calloc_atomic(size_t n)
{
  void *p;
//  p = GC_malloc_atomic(n);
  p = malloc(n);
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
//            t = q1->next; GC_free(q1->m); GC_free(q1); q1 = t;
            t = q1->next; free(q1->m); free(q1); q1 = t;
          }
//          t = q2->next; GC_free(q2->m); GC_free(q2); q2 = t;
          t = q2->next; free(q2->m); free(q2); q2 = t;
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

// p2 should be a constant
Poly divc_poly(Poly p1,Poly p2)
{
  struct poly root;
  Poly p,q,r,s;
  Coef c,c1;
  Monomial m;

  if ( p2 == 0 )
    error("divc_poly : division by 0");
  if ( p2->m->td != 0 )
    error("divc_poly : division by a non constant poly");
  if ( CurrentRing->divc == 0 )
    error("divc_poly : division is not allowed");
  r = &root;
  c = p2->c;
  for ( q = p1; q != 0; q = q->next ) {
     c1 = CurrentRing->divc(q->c,c); 
     APPENDPOLY(r,s,c1,p1->m);
  }
  r->next = 0;
  return root.next;
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
  Coef c;
  Poly r;
  Monomial m;

  c = CurrentRing->ntoc(n);
  if ( c.f == 0 ) return 0;
  else {
    NEWPOLY(r);
    r->c = c;
    NEWMONOMIAL(m);
    m->td = 0;
    r->m = m;
    return r;
  }
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
  m->exp[wpos] = ((ULONG)1)<<(shift*8);
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
//  r->vname = (char **)GC_malloc(r->nv*sizeof(char *));
  r->vname = (char **)calloc(r->nv*sizeof(char *),1);
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

void show_ring(Ring r)
{
  char **v;
  int n,i;
  char *ordtype,*maxexp;

  switch ( r->chr ) {
    case 0: fprintf(stderr,"ring=Z["); break;
    case 1: fprintf(stderr,"ring=Q["); break;
    default: fprintf(stderr,"ring=(Z/%dZ)[",(int)r->chr); break;
  }
  v = r->vname; n = r->nv;
  for ( i = 0; i < n; i++ ) {
    fprintf(stderr,"%s",v[i]);
    if ( i < n-1 ) fprintf(stderr,",");
  }
  if ( r->graded )
    ordtype = r->rev?"grevlex":"glex";
  else
    ordtype = "lex";
  switch ( r->bpe ) {
    case 1: maxexp = "127"; break;
    case 2: maxexp = "32767"; break;
    case 4: maxexp = "2147483647"; break;
    case 8: maxexp = "9223372036854775807"; break;
    default: maxexp = "?"; break;
  }
  fprintf(stderr,"],ordtype=%s,max exponent=%s\n",ordtype,maxexp);
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
  int nv,bpe,rev,i,shift,bpos,wpos,first=1;
  char **v;

  nv = CurrentRing->nv; bpe = CurrentRing->bpe;
  v = CurrentRing->vname; rev = CurrentRing->rev;
  if ( bpe == 8 ) mask = ~0;
  else mask = (((ULONG)1)<<(bpe*8))-1;
  for ( i = 0; i < nv; i++ ) {
    bpos = rev ? (nv-i-1)*bpe : i*bpe; 
    wpos = bpos/8; shift = 8-bpe-(bpos%8);
    e = (m->exp[wpos] >> (shift*8))&mask;
    if ( e != 0 ) {
      if ( first ) first = 0;
      else printf("*");
      printf("%s",v[i]);
      if ( e > 1 ) printf("^%lld",e);
    }
  }
}

void print_poly(Poly p)
{
  Poly q;

  if ( p == 0 ) {
    printf("0");
    return;
  }
  for ( q = p; q != 0; q = q->next ) {
    printf("+("); CurrentRing->printc(q->c); printf(")");
    if ( q->m->td != 0 ) {
      putchar('*'); print_monomial(q->m);
    }
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
//      s = (char *)GC_malloc(i+1);
      s = (char *)calloc(i+1,1);
      strcpy(s,buf);
      APPENDNODE(p,p1,s);
    }
  }
}

// create [x,y,z]
Node default_vars()
{
  char xyz[] = "xyz";
  int len = strlen(xyz),i;
  Node top,cur;
  char *s;

  for ( top = 0, i = len-1; i >= 0; i-- ) {
//    s = (char *)GC_malloc(2);
    s = (char *)calloc(2,1);
    s[0] = xyz[i]; s[1] = 0;
    CONSNODE(top,cur,s);
  }
  return top;
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
//  return (void *)GC_realloc(p,nsize);
  return (void *)realloc(p,nsize);
}

int skipspace_str(char *str, int j)
{
  while ( str[j]  == ' ' || str[j]  == '\t' || str[j] == '\r' || str[j] == '\n' ) j++;
  return j;
}

char *print_monomial_str(Monomial m)
{
  ULONG mask,e;
  int nv,bpe,rev,i,shift,bpos,wpos,first=1,vlen=0,elen=0;
  char **v;
  char *buf,*tmp;

  nv = CurrentRing->nv; bpe = CurrentRing->bpe;
  v = CurrentRing->vname; rev = CurrentRing->rev;
  for ( i = 0; i < nv; i++ ) vlen = vlen + strlen(v[i]);
  switch ( bpe ) { 
    case 1: elen = 3; break;
    case 2: elen = 5; break;
    case 4: elen = 10; break;
    case 8: elen = 20; break;
    default: break;
  }
//  buf = (char *)GC_malloc( vlen + (2 + elen) * nv + 1 ); // *v[0]^e*v[1]^e...
//  tmp = (char *)GC_malloc( elen + 2 );
  buf = (char *)calloc( vlen + (2 + elen) * nv + 1,1 ); // *v[0]^e*v[1]^e...
  tmp = (char *)calloc( elen + 2,1 );
  sprintf(buf,""); sprintf(tmp,"");
  if ( bpe == 8 ) mask = ~0;
  else mask = (((ULONG)1)<<(bpe*8))-1;
  for ( i = 0; i < nv; i++ ) {
    bpos = rev ? (nv-i-1)*bpe : i*bpe; 
    wpos = bpos/8; shift = 8-bpe-(bpos%8);
    e = (m->exp[wpos] >> (shift*8))&mask;
    if ( e != 0 ) {
      if ( first ) first = 0;
      else  strcat(buf, "*");
      strcat(buf, v[i]);
      if ( e > 1 ) {
        sprintf(tmp,"^%"PRIu64, e);
        strcat(buf, tmp);
        sprintf(tmp,"");
      }
    }
  }
  return buf;
}

char *print_poly_str(Poly p)
{
  Poly q;
  char *pbuf;
  int len = 0;
  for ( q = p; q != 0; q = q->next ) {
    len = len + strlen(CurrentRing->printc(q->c)) + 3;
    if ( q->m->td != 0 ) {
      len = len + strlen(print_monomial_str(q->m)) + 1;
    }
  }
//  pbuf = (char *)GC_malloc(len + 1);
  pbuf = (char *)calloc(len + 1,1);
  sprintf(pbuf,"");

  if ( p == 0 ) {
    return "0";
  }
  for ( q = p; q != 0; q = q->next ) {
    strcat(pbuf, "+("); strcat(pbuf, CurrentRing->printc(q->c)); strcat(pbuf, ")");
    if ( q->m->td != 0 ) {
      strcat(pbuf, "*"); strcat(pbuf, print_monomial_str(q->m));
    }
  }
//  printf("yyparse result:%s\n",pbuf);  
  return pbuf;
}

Node get_vars_str(char *str)
{
  struct node root;
  Node p,p1;
  int c,i;
  char buf[BUFSIZ];
  char *s;
  int j = 0;

  p = &root;
  while ( str[j++] != '[' );
  while ( 1 ) {
    j = skipspace_str(str, j);
    c = str[j];
    if ( c == ']' ) {
      p->next = 0;
      return root.next;
    } else {
      buf[0] = c;
      for ( i = 1; ; i++ ) {
        if ( i == BUFSIZ )
          error("get_vars : variable name too long");
        j++;
        c = str[j];
        if ( !isalnum(c) ) {
          buf[i] = 0;
          break;
        } else
          buf[i] = c;
      }
//      s = (char *)GC_malloc(i+1);
      s = (char *)calloc(i+1,1);
      strcpy(s,buf);
      APPENDNODE(p,p1,s);
    }
  }
}

int set_parameters(char *v, int o, int b, int c)
{ 
  Node vars;
  vars = get_vars_str(v);
  CurrentRing = create_ring(vars,o,b,c);
  show_ring(CurrentRing);
  return 1;
}

char *yyparse_input(char *str)
{
  set_str(str);
  yyparse();
  return print_poly_str(result);
}

int main(int argc,char **argv)
{
  int chr,ordid,bpe;
  char vbuf[128],buf[256];
/*
  GC_init();
  mp_set_memory_functions(
    (void *(*)(size_t))GC_malloc,
    (void *(*)(void *,size_t,size_t))gc_realloc,
    (void (*)(void *,size_t))GC_free);
*/
  printf("chr:");scanf("%d", &chr);
  printf("ordid:");scanf("%d", &ordid);
  printf("bpe:");scanf("%d", &bpe);
  scanf("%*c");
  printf("variables:");
  if (scanf("%255[^\n]%*[^\n]", vbuf) != 1) {
      return 1;
  }
  set_parameters(vbuf, ordid, bpe, chr);
//  show_ring(CurrentRing);
  while ( 1 ) {
    scanf("%*c");
    printf("Input: ");
    if (scanf("%255[^\n]%*[^\n]", buf) != 1) {
      return 1;
    }
    printf("%s\n",yyparse_input(buf));
  }

}

char *c_exec(char *v, int o, int b, int c, char *str){
/*
  GC_init();
  mp_set_memory_functions(
    (void *(*)(size_t))GC_malloc,
    (void *(*)(void *,size_t,size_t))gc_realloc,
    (void (*)(void *,size_t))GC_free);
  printf("GC_init() was executed!\n");
*/
  Node vars = get_vars_str(v);
  CurrentRing = create_ring(vars,o,b,c);
  show_ring(CurrentRing);
  set_str(str);
  yyparse();
  return print_poly_str(result);
}

