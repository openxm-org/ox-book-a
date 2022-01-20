#include <stdio.h>
#include <math.h>
double bisection(double left,double right) {
   double epsilon = 0.00001;
   double c;
   double fc;
   double t1,t2;
   if ((t1=(tan(left)-left) < 0) && 
       (t2=(tan(right)-right) > 0)) {
   }else{
     fprintf(stderr,"Error: f(left) < 0 and f(right) > 0 must hold.");
     fprintf(stderr," t1=%lf, t2=%lf\n",t1,t2);
     return(0.0);
   }
   while (1) {
     c = (left+right)/2.0;
     if (fabs(right-left)<epsilon) {
       return(c);
     }
     fc = tan(c)-c;
     if (fc > 0) { right = c; }
     else if (fc < 0) { left = c; }
     else if (fc==0.0) return(c);
   }
}

int main() {
  int i;
  double left=3.14;
  double right=3.14+3.14/2.0;
  double zp;
  double zp0;
  double x;
  printf("0  1\n");
  zp0=0.001;
  for (i=1; i<30; i++) {
    zp = bisection(left,right);
    for (x=zp0; x<=zp; x += 0.3) { 
      printf("%lf %lf\n",x,sin(x)/x); // [zp0,zp]  の間で値を計算.
    }
    left = 3.14*(i+1);
    right = M_PI*(i+1)+3.14/2.0;  // 3.14*(i+1) ではiが大きくなるとだめ.
    zp0=zp;
  }
}
