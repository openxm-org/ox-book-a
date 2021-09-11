#include <stdio.h>
#include <gmp.h>

int main()
{
	int m=31,n=7,exp=100;
	mpz_t a,b,c;
	mpz_init(a);      
	mpz_init_set_ui(b,m);  /* b:=31 */
	mpz_init_set_ui(c,n);  /* c:=7 */

	mpz_pow_ui(a,b,exp);   /* a:= b^100 */
	gmp_fprintf(stdout,"%d^%d == %Zd\n",m,exp,a);

	mpz_powm_ui(a,b,exp,c);   /* a:= b^100 mod c*/
	gmp_fprintf(stdout,"%d^%d mod %d == %Zd\n",m,exp,n,a);
	return 0;
}
