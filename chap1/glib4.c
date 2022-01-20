/* glib4.c
   2019.12.02
*/
/* gcc -DTEST -I/opt/X11/include -L/opt/X11/lib -lX11 glib4.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xlocale.h>

#define DEFAULT_FONT_NAME       "-*-*-*-R-Normal--14-130-75-75-*-*"
#define DEFAULT_FONT_NAME2       "*fixed*"
#define F_SIZE                  14
#define COL                     30
#define ROW                     10

#include "glib4.h"

#ifdef TEST
int main() {
  glib_open();
  glib_loop();
}
void glib_draw() { // 描画の手続きはこの中に書く.
  double y;
  glib_window(0.0, 0.0, 1.0, 1.0);
  for (y=0.0; y<1.0; y += 0.1) {
    glib_color(NULL,0x8a0000+(int)(y*1000)); // 色の設定
    glib_line(0.0,0.0, 0.7, y);
  }
  glib_color("black",0);
  glib_text(0.1,0.1,"Hello");
  glib_flush();
}
#endif


double Glib_xmin, Glib_xmax, Glib_ymin, Glib_ymax;

void glib_open() { gopen(); }

/* -------------------- main code of glib.c ---------------- */
#define XSIZE 600
#define YSIZE 600
#define P(x)  (((x-Glib_xmin)/(Glib_xmax-Glib_xmin))*(double)XSIZE)
#define Q(y)  ((double)YSIZE-((y-Glib_ymin)/(Glib_ymax-Glib_ymin))*(double)YSIZE)


/* global variables */
Display *Glib_d;
int Glib_s;
Window Glib_w;
GC Glib_gc;
XSetWindowAttributes Glib_a;
Atom                    Glib_protocols;
XWMHints                Glib_hints;
XFontSet                Glib_fs;
XFontSetExtents         *Glib_fs_ext;
int                     Glib_dec;
XEvent                  Glib_event;
unsigned long           Glib_mask;


static int glib_x = 0;
static int glib_y = 0;



#define G3_TSIZE 100
#define G3_XSIZE 100
#define G3_P(t)  (((t-Glib_xmin)/(Glib_xmax-Glib_xmin))*(double)G3_TSIZE)
#define G3_Q(x)  ((double)G3_XSIZE-((x-Glib_ymin)/(Glib_ymax-Glib_ymin))*(double)G3_XSIZE)

double G3_u[G3_TSIZE][G3_XSIZE];
int    G3_f[G3_TSIZE][G3_XSIZE];
double Glib3_theta_xt = -30.0;
double Glib3_theta_y  =  30.0; 


void glib3_init() {
  int i,j;
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  G3_f[i][j] = 0;
	}
  }
}

void glib3_init_test() {
  int i,j;
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  if ((i*j) % 5 == 0) {
		G3_f[i][j] = 1; G3_u[i][j] = j;
	  }else{
		G3_f[i][j] = 0;
	  }
	}
  }
}

void glib3_putpixel(double t,double x, double u) {
  int i,j;
  i = (int) (G3_P(t));
  j = (int) (G3_Q(x));
  if ((0 <= i) && (i < G3_TSIZE) && (0 <= j) && (j < G3_XSIZE)) {
	G3_u[i][j] = u; G3_f[i][j] = 1;
  }
}

int glib3_print_value(int step,int c) {
  double umax, umin;
  double xratio, yratio;
  int pmax = 0;
  int first =  1;
  int i,j,p,q,p2,q2;
  int cmax,cc;
  if (!c) {
	for (i=0; i<G3_TSIZE; i += step) {
	  for (j=0; j<G3_XSIZE; j += step) {
		if (G3_f[i][j]) {
		  printf("%03.1f ",G3_u[i][j]);
		}else{
		  printf("???.? ");
		}
	  }
	  printf("\n");
	}
	return 0;
  }

  cmax = glib_color_max();
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  if (G3_f[i][j]) {
		pmax++;
		if (first) {
		  first = 0;
		  umax = umin = G3_u[i][j];
		}else{
		  if (G3_u[i][j] > umax) umax = G3_u[i][j];
		  if (G3_u[i][j] < umin) umin = G3_u[i][j];
		}
	  }
	}
  }
  printf("t:horizontal, x: vertical. points=%d, umin=%f, umax=%f\n",pmax,umin,umax);
  xratio = ((double) XSIZE)/((double) G3_TSIZE);
  yratio = ((double) YSIZE)/((double) G3_XSIZE);
  for (i=0; i<G3_TSIZE; i += step) {
	for (j=0; j<G3_XSIZE; j += step) {
	  if (G3_f[i][j]) {
		cc=(int) (0.9*((double)cmax)*(G3_u[i][j]-umin)/(umax-umin));
		glib_color(NULL,cc);
		printf("%010d ",cc);
	  }else printf("?????????? ");
	}
	printf("\n");
  }
  return(0);
}

void glib3_interpolate() {
  int i,j,flag;
  double u;
  for (i=0; i<G3_TSIZE; i++) {
    flag = 0;
	for (j=0; j<G3_XSIZE; j++) {
	  if (G3_f[i][j]) {
		flag = 1; u = G3_u[i][j];
	  }else if (flag) { G3_u[i][j]= u; G3_f[i][j] = 2;}
	}
  }

/*
  for (j=0; j<G3_XSIZE; j++) {
    flag = 0;
	for (i=0; i<G3_TSIZE; i++) {
	  if (G3_f[i][j]) {
		flag = 1; u = G3_u[i][j];
	  }else if (flag) { G3_u[i][j] = u; G3_f[i][j] = 3; }
	}
  }
*/
}

void glib3_contour_color(int level) {
  double umax, umin;
  double xratio, yratio;
  int pmax = 0;
  int first =  1;
  int i,j,p,q,p2,q2;
  int cmax,cc;
  int quantize;
  cmax = glib_color_max();
  if (cmax < level) fprintf(stderr,"cmax=%d is too small.\n",cmax);
  quantize = cmax/level;
  if (cmax < level) {
    fprintf(stderr,"cmax=%d is too small.\n",cmax);
    quantize = 1;
  }
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  if (G3_f[i][j]) {
		pmax++;
		if (first) {
		  first = 0;
		  umax = umin = G3_u[i][j];
		}else{
		  if (G3_u[i][j] > umax) umax = G3_u[i][j];
		  if (G3_u[i][j] < umin) umin = G3_u[i][j];
		}
	  }
	}
  }
  printf("t:horizontal, x: vertical. points=%d, umin=%f, umax=%f\n",pmax,umin,umax);
  xratio = ((double) XSIZE)/((double) G3_TSIZE);
  yratio = ((double) YSIZE)/((double) G3_XSIZE);
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  if (G3_f[i][j]) {
		p = (int) (((double)i)*xratio);
		p2 = (int) (((double)(i+1))*xratio)-1;
		q = (int) (((double)j)*yratio);
		q2 = (int) (((double)(j+1))*yratio)-1;
		if (p2-p == 0) p2=p+1;
		if (q2-q == 0) q2=q+1;
		cc=(int) (0.9*((double)cmax)*(G3_u[i][j]-umin)/(umax-umin));
        cc = cc/quantize;
        cc = quantize*cc;
		glib_color(NULL,cc);
		/* printf("%010d ",cc); */
		XFillRectangle(Glib_d,Glib_w,Glib_gc,p,q,p2-p,q2-q);
	  }
	}
	/* printf("\n"); */
  }
}

void glib3_plot3d_slice(int level) {
  double umax, umin;
  double xratio, yratio;
  int pmax = 0;
  int first =  1;
  int i,j,p,q,p2,q2;
  int cmax,cc;
  int quantize;
  int ymin[XSIZE];
  int ymax[XSIZE];
  double theta_xt;
  double theta_y;
  double c1,c2,s1,s2;
  int y;
  int px,py;
  double pxx, pyy;
  int xs,ys;
  int istep, jstep, jinit;
  theta_xt = Glib3_theta_xt;
  theta_y  = Glib3_theta_y;
#define RD 3.141592/180
  c1 = cos(theta_xt*RD); s1=sin(theta_xt*RD);
  c2 = cos(theta_y*RD);  s2=sin(theta_y*RD); 

  for (i=0; i<XSIZE; i++) {
	ymin[i] = YSIZE-1; ymax[i] = 0;
  }
  cmax = glib_color_max();
  if (cmax < level) fprintf(stderr,"cmax=%d is too small.\n",cmax);
  quantize = cmax/level;
  if (cmax < level) {
    fprintf(stderr,"cmax=%d is too small.\n",cmax);
    quantize = 1;
  }
  for (i=0; i<G3_TSIZE; i++) {
	for (j=0; j<G3_XSIZE; j++) {
	  if (G3_f[i][j]) {
		pmax++;
		if (first) {
		  first = 0;
		  umax = umin = G3_u[i][j];
		}else{
		  if (G3_u[i][j] > umax) umax = G3_u[i][j];
		  if (G3_u[i][j] < umin) umin = G3_u[i][j];
		}
	  }
	}
  }
  printf("theta_xt=%f, theta_y=%f, cmax=%x, points=%d, umin=%f, umax=%f\n",
		 theta_xt, theta_y,cmax,pmax,umin,umax);
  xratio = ((double) XSIZE)/((double) G3_TSIZE);
  yratio = ((double) YSIZE)/((double) G3_XSIZE);
  xs = (int) (((double)(1))*xratio) -1;
  ys = (int) (((double)(1))*yratio) -1;
  if (xs <= 0) xs = 1;
  if (ys <= 0) ys = 1;

#define rotx(p,q)  (  c1*(q-YSIZE/2)  + s1*(p-XSIZE/2)            + XSIZE/2 )
#define roty(p,q)  ( (-s1*(q-YSIZE/2) + c1*(p-XSIZE/2))*s2 + y*c2 + YSIZE/2 )
#define uyyy(u)    (-((double)YSIZE/3)*(u-umin)/(umax-umin))

  /* draw frame */
  glib3_draw_frame(c1,s1,c2,s2,umin,umax,y);

  if (-s1 < 0) { jstep = 1; jinit= 0;} else {jstep = -1; jinit = G3_XSIZE-1; }
  if (c1  < 0) { istep = 2; i = 0; } else {istep = -2; i = G3_TSIZE-1; }
  printf("i=%d, istep=%d, j=%d, jstep=%d\n",i,istep,j,jstep);
  while (i>=0 && i<G3_TSIZE) {  /* t */
    j = jinit;
	while (j>=0 && j<G3_XSIZE) { /* x */
	  if (G3_f[i][j]) {
		p = (int) (((double)i)*xratio);
		q = (int) (((double)j)*yratio);
		cc=(int) (0.9*((double)cmax)*(G3_u[i][j]-umin)/(umax-umin));
        cc = cc/quantize;
        cc = quantize*cc;
		y = (int) uyyy(G3_u[i][j]);
		glib_color(NULL,cc);
        /* printf("(p,q,y)=(%d,%d,%d)\n",p,q,y); */
        pxx = rotx(p,q);
        pyy = roty(p,q);  /* It uses the value of y */
        px = pxx; py = pyy;
        if (px >=0 && px < XSIZE) {
		  if (py < ymin[px]) {
			ymin[px] = py;
			XFillRectangle(Glib_d,Glib_w,Glib_gc,px,py,xs,ys);
		  }
		  if (py > ymax[px]) {
			ymax[px] = py;
			XFillRectangle(Glib_d,Glib_w,Glib_gc,px,py,xs,ys);
		  }
		}
	  }
	  j += jstep;
	}
	i += istep;
  }
}

/* Draw window frame */
void glib3_draw_frame(double c1,double s1,double c2,double s2,double umin,double umax,int y) {
  int px,py,p,q;
  y = (int) uyyy(0.0); 
  px = (int) rotx(0,0);     py = (int) roty(0,0);   
  p  = (int) rotx(XSIZE,0); q = (int) roty(XSIZE,0);
  glib_color("red",0);
  XDrawLine(Glib_d,Glib_w,Glib_gc,px,py,p,q);

  px = (int) rotx(0,YSIZE);     py = (int) roty(0,YSIZE);   
  p  = (int) rotx(XSIZE,YSIZE); q = (int) roty(XSIZE,YSIZE);
  glib_color("red",0);
  XDrawLine(Glib_d,Glib_w,Glib_gc,px,py,p,q);

  px = (int) rotx(0,0);     py = (int) roty(0,0);   
  p  = (int) rotx(0,YSIZE); q = (int) roty(0,YSIZE);
  glib_color("blue",0);
  XDrawLine(Glib_d,Glib_w,Glib_gc,px,py,p,q);

  px = (int) rotx(XSIZE,0);     py = (int) roty(XSIZE,0);   
  p  = (int) rotx(XSIZE,YSIZE); q = (int) roty(XSIZE,YSIZE);
  glib_color("blue",0);
  XDrawLine(Glib_d,Glib_w,Glib_gc,px,py,p,q);
}

/* functions exported */
void gopen() {
  static int init=0;
  static char                    **missing_list;
  static int                     missing_count;
  static char                    *def_string;

  if (init) return; else init=1;
  
  Glib_d = XOpenDisplay(NULL);
  Glib_s = DefaultScreen(Glib_d);
  Glib_gc = DefaultGC(Glib_d,Glib_s);

  Glib_fs = XCreateFontSet(Glib_d, DEFAULT_FONT_NAME, &missing_list,
                           &missing_count, &def_string);
  if (Glib_fs == NULL) {
    fprintf(stderr,"Trying other fonts...\n");
    Glib_fs = XCreateFontSet(Glib_d, DEFAULT_FONT_NAME2, &missing_list,
			     &missing_count, &def_string);
    if (Glib_fs == NULL) {
      fprintf(stderr,"Error: execute xlsfonts and set a relevant font name to DEFAULT_FONT_NAME in glib4.c\n");
      exit(-1);
    }
  }
  Glib_fs_ext = XExtentsOfFontSet(Glib_fs);
  Glib_dec = Glib_fs_ext->max_logical_extent.height-(-Glib_fs_ext->max_logical_extent.y);

  Glib_w = XCreateSimpleWindow(Glib_d,RootWindow(Glib_d,Glib_s),0,0,
			  XSIZE,YSIZE,2,BlackPixel(Glib_d,Glib_s),WhitePixel(Glib_d,Glib_s));

  XStoreName(Glib_d, Glib_w, "glib");
  Glib_hints.flags = InputHint;
  Glib_hints.input = True;
  XSetWMHints(Glib_d, Glib_w, &Glib_hints);
  Glib_protocols = XInternAtom(Glib_d, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(Glib_d, Glib_w, &Glib_protocols, 1);

  Glib_mask = ExposureMask | KeyPressMask | FocusChangeMask | ButtonPressMask
	| ButtonReleaseMask | Button1MotionMask;
  XSelectInput(Glib_d, Glib_w, Glib_mask);

  /*  xsize, ysize, border color, border, background */
  /* Glib_a.override_redirect = 1;
	 XChangeWindowAttributes(Glib_d,Glib_w,CWOverrideRedirect, &Glib_a);  */

  XMapWindow(Glib_d,Glib_w);

  /* intialize line attributes */
  XSetLineAttributes(Glib_d,Glib_gc,1,LineSolid,CapButt,JoinMiter);
}

void glib_loop() {
  static int mx,my;
  static int mxNew,myNew;
  Window subw;
  printf("Type in ctrl-C to exit.%c",0xa);
  for(;;){
	XNextEvent(Glib_d, &Glib_event);
	if(XFilterEvent(&Glib_event, (Window) NULL) == True)
	  continue;
	switch(Glib_event.type){
	case Expose : 
      glib_draw();
      break;
	case KeyPress : break;
	case ButtonPress:
	  mx = Glib_event.xbutton.x;
	  my = Glib_event.xbutton.y;
	  break;
    case ButtonRelease:
	  mxNew = Glib_event.xbutton.x;
	  myNew = Glib_event.xbutton.y;
	  Glib3_theta_xt += (mxNew-mx)/10 + 1;
	  Glib3_theta_y += (myNew-my)/10 ;
	  {
		Colormap cmap;
		XColor c0, c1;
		cmap = DefaultColormap(Glib_d,Glib_s);
		XAllocNamedColor(Glib_d,cmap,"white",&c1,&c0);
		XSetForeground(Glib_d,Glib_gc,c1.pixel);
		XFillRectangle(Glib_d,Glib_w,Glib_gc,0,0,XSIZE,YSIZE);
	  }
	  glib_draw();
	  break;
	case ClientMessage :
	  if(Glib_event.xclient.data.l[0] == Glib_protocols){
		XDestroyWindow(Glib_d, Glib_w);
		XCloseDisplay(Glib_d);
		exit(0);
	  }
	  break;
	defaults:
	  break;
	}
  }
}

void gclose() {
  XFlush(Glib_d);
}

void gFlush() {
  XFlush(Glib_d);
}


void moveto(x,y)
int x,y;
{
  glib_x = x;
  glib_y = y;
}

void lineto(x,y)
int x,y;
{
  XDrawLine(Glib_d,Glib_w,Glib_gc,glib_x,glib_y,x,y);
  glib_x = x; glib_y = y;
}

void setpixel(x,y)
int x,y;
{
  XDrawPoint(Glib_d,Glib_w,Glib_gc,x,y);
}


void glib_window(double xmin,double ymin, double xmax, double ymax) {
  Glib_xmin = xmin;
  Glib_ymin = ymin;
  Glib_xmax = xmax;
  Glib_ymax = ymax;
  glib3_init();
  gopen();
}

void glib_line(double x0,double y0, double x1, double y1) {
  int p0,q0,p1,q1;
  p0 = (int) P(x0);
  q0 = (int) Q(y0);
  p1 = (int) P(x1);
  q1 = (int) Q(y1);
  moveto(p0,q0);
  lineto(p1,q1);
}

void glib_text(double x0,double y0, char s[]) {
  int p0,q0;
  p0 = (int) P(x0);
  q0 = (int) Q(y0);
  XDrawString(Glib_d, Glib_w, Glib_gc, p0,q0,s,strlen(s));
  /* Glib_fs is used to use XmbDrawString */
}

void glib_putpixel(double x0,double y0) {
  int p0,q0;
  p0 = (int) P(x0);
  q0 = (int) Q(y0);
  setpixel(p0,q0);  
}

void glib_flush() {
  gFlush();
}


void glib_clear() {
  XClearArea(Glib_d,Glib_w,0,0,XSIZE,YSIZE,False);
}

void glib_color(char color[],int c) {
  Colormap cmap;
  XColor c0, c1;
  if (color != NULL) {
	cmap = DefaultColormap(Glib_d,Glib_s);
	XAllocNamedColor(Glib_d,cmap,color,&c1,&c0);
	XSetForeground(Glib_d,Glib_gc,c1.pixel);
  }else{
	XSetForeground(Glib_d,Glib_gc,c);
  }
}
int glib_color_max() {
  static int m=0;
  Colormap cmap;
  XColor c0, c1;
  if (m != 0) return m;
  cmap = DefaultColormap(Glib_d,Glib_s);
  XAllocNamedColor(Glib_d,cmap,"white",&c1,&c0);
  m=c1.pixel;
  if (m == 0) {
	fprintf(stderr,"glib_color_max(): white is 0?\n");
	m = 1;
  }
  return m;
}

void glib_set_frame_no(int n) {
  /* not implemented */
}

void glib_init_frame_no(int n) {
  /* not implemented */
}

void glib_close() {
  gclose();
}

void glib_cgif_pen_up() {
  /* do nothing */
}


#ifdef TEST1
int main() {
  glib_open();
  glib_loop();
}

void glib_draw() {
  double y;
  glib_window(0.0, 0.0, 1.0, 1.0);
  for (y=0.0; y<1.0; y += 0.1) {
	glib_color(NULL,0x8a0000+(int)(y*1000));
	glib_line(0.0,0.0, 0.7, y);
  }
  glib_color("black",0);
  glib_text(0.1,0.1,"Hello");
  glib_flush();
}
#endif

#ifdef TEST2
int main() {
  glib_open();
  glib3_init_test();
  glib_loop();
}

void glib_draw() {
  glib_window(0.0, 0.0, 1.0, 1.0);
  glib3_contour_color(3);
  glib_flush();
}
#endif

#ifdef TEST3
int main() {
  double t,x;
  glib_window(0.0, 0.0, 1.0, 1.0);
  for (t = 0.0; t<=0.7; t += 0.005) {
	for (x = 0.0; x <= 0.7; x += 0.005) {
	  if (t*t+x*x < 0.08) glib3_putpixel(t,x,0.0);
	  else glib3_putpixel(t,x,t*t+x*x);
	}
  }
  /* glib3_print_value(5,1); */
  glib_loop();
}

void glib_draw() {
  glib3_contour_color(65);
  glib_flush();
}
#endif

#ifdef TEST4
int main() {
  glib_open();
  glib_loop();
}
void glib_draw() {
  glib_window(0,0,5,5);
  glib_color(NULL,0xff0000);
  glib_line(0,0,4.5,3);
}
#endif


