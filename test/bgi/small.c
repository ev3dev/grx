#ifdef __GNUC__
#  include <libbcc.h>
#else
#  include <graphics.h>
#endif

void main(void)
{
  int  gd, gm, err;

  gd = DETECT;
  initgraph(&gd,&gm,"d:\\tp\\bgi");
  err = graphresult();
  closegraph();
}
