#include "calc.h"

int main(int argc,char **argv)
{
  char *ringfile;
  char buf[BUFSIZ];
  Poly p;
  int from_string;

  if ( argc == 1 )
    ringfile = 0;
  else if ( !strcmp(argv[1],"-s") ) {
    from_string = 1;
    ringfile = argv[2];
  } else {
    from_string = 0;
    ringfile = argv[1];
  }
  init_calc(ringfile,from_string);
  show_ring(CurrentRing);
  Input = stdin;
  while ( 1 ) {
    fgets(buf,sizeof(buf),stdin);
    p = eval_string(buf);
    print_poly(p); printf("\n");
  }
}

