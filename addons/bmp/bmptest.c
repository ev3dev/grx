#include "grx-3.0.h"
#include "bmp.c"

int main ( void )
{
  GrBmpImage *bmp256, *bmp;
  GrxPixmap *p256, *p;
  GError error = NULL;
  if (!grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR, &error, 800, 600, 256)) {
    g_error("%s", error->message);
  }
  bmp256 = GrLoadBmpImage("mysha256.bmp");
  bmp = GrLoadBmpImage("some1.bmp");
  GrAllocBmpImageColors(bmp, NULL);
  GrAllocBmpImageColors(bmp256, NULL);
  p256 = GrConvertBmpImageToPattern(bmp256);
  p = GrConvertBmpImageToPattern(bmp);
  if ( p ) {
    grx_draw_pixmap(0, 0, p);
    getkey();
  }
  if ( p256 ) {
    grx_draw_pixmap(300, 300, p256);
    getkey();
  }
  if ( p ) grx_pixmap_free(p);
  if ( p256 ) grx_pixmap_free(p256);
  GrSaveBmpImage("save.bmp", NULL, 0, 0, 400, 400);
  GrUnloadBmpImage(bmp);
  GrUnloadBmpImage(bmp256);
  return 0;
};

