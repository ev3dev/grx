#include <libbcc.h>

int main(void)
{
  int  gd, gm, err;

  gd = DETECT;
  initgraph(&gd,&gm,"..\\..\\chr");
  err = graphresult();
  closegraph();
  return 0;
}
