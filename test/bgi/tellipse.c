#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __GNUC__
#  include <libbcc.h>
#  ifdef __linux__
     extern int kbhit(void), getkey(void);
#  else
#    include <pc.h>
#  endif
#  define getch() getkey()
#else
#  include <graphics.h>
#  include <conio.h>
#endif

#if defined(__WIN32__)
int GRXMain(void)
#else
int main(void)
#endif
{
  int  gd, gm;
  int  err;
  int  x, y, xr, i;

  gd = DETECT;
  initgraph(&gd,&gm,"d:\\tp\\bgi");
  err = graphresult();
  if (err != grOk) {
    fprintf(stderr, "Couldn't initialize graphics\n");
    exit(1);
  }
  x = getmaxx()/2;
  y = getmaxy()/2;
  for (i=-10; i <= 10; i+=2) {
    cleardevice();
    for (xr=1; xr <= x && xr < y ; xr += x/16)
      ellipse(x,y,0,360+i,xr,xr*y/x);
    getch();
  }
  for (i=1; i <= 10; i++) {
    cleardevice();
    for (xr=1; xr <= x && xr < y ; xr += x/16)
      ellipse(x,y,0,360*i,xr,xr*y/x);
    getch();
  }
  closegraph();
  return 0;
}
