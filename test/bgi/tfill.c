#include <stdio.h>
#include <stdlib.h>
#ifdef __GNUC__
#  include <libbcc.h>
#  ifdef __MSDOS__
#    include <pc.h>
#  else
     extern int kbhit(void);
     extern int getkey(void);
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
  int gd, gm;
  int err;

  gd = DETECT;
  initgraph(&gd,&gm,"c:\\bc\\bgi");
  err = graphresult();
  if (err != grOk) {
    fprintf(stderr, "Couldn't initialize graphics\n");
    exit(1);
  }
  setviewport( 100, 100, 200, 200, 1);
  floodfill( 10, 10, WHITE);
  getch();
  setfillstyle( SOLID_FILL, DARKGRAY);
  cleardevice();
  setviewport( 100, 100, 200, 200, 0);
  floodfill( 10, 10, WHITE);
  getch();
  closegraph();
  return 0;
}
