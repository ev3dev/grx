#include <libbcc.h>

int main(void)
{
  int  gd, gm, err;

  gd = DETECT;
  initgraph(&gd,&gm,"d:\\tp\\bgi");
  err = graphresult();
  closegraph();
  return 0;
}
