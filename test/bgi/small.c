#ifdef __GNUC__
#  include <libbcc.h>
#else
#  include <graphics.h>
#endif

#if defined(__WIN32__)
int GRXMain(void)
#else
int main(void)
#endif
{
  int  gd, gm, err;

  gd = DETECT;
  initgraph(&gd,&gm,"d:\\tp\\bgi");
  err = graphresult();
  closegraph();
  return 0;
}
