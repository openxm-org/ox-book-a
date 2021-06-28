void gopen();
void glib_loop();
void glib_open(); 
void glib_window(double xmin,double ymin, double xmax, double ymax);
void glib_line(double x0,double y0, double x1, double y1);
void glib_text(double x0,double y0, char s[]);
void glib_putpixel(double x0,double y0);
void glib_set_point(double x, double y);
void glib_flush();
void glib_clear();
void glib_color(char color[],int c);
void glib_close();

void glib_cgif_pen_up();
int glib_color_max();
void glib_set_frame_no(int n);
void glib_init_frame_no(int n);
void glib_draw();

void moveto(int x,int y);
void lineto(int x,int y);
void setpixel(int x,int y);


void glib3_init();
void glib3_init_test();
void glib3_putpixel(double t,double x, double u);
int glib3_print_value(int step,int c);
void glib3_interpolate();
void glib3_contour_color(int level);
void glib3_plot3d_slice(int level);
void glib3_draw_frame(double c1,double s1,double c2,double s2,double umin,double umax,int y);
void gclose();
void gFlush();
