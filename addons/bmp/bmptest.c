#include "grx-3.0.h"
#include "bmp.c"

int main ( void )
{
  GrBmpImage *bmp256, *bmp;
  GrxPattern *p256, *p;
  grx_set_mode(GRX_GRAPHICS_MODE_GRAPHICS_WIDTH_HEIGHT_COLOR, 800, 600, 256);
  bmp256 = GrLoadBmpImage("mysha256.bmp");
  bmp = GrLoadBmpImage("some1.bmp");
  GrAllocBmpImageColors(bmp, NULL);
  GrAllocBmpImageColors(bmp256, NULL);
  p256 = GrConvertBmpImageToPattern(bmp256);
  p = GrConvertBmpImageToPattern(bmp);
  if ( p ) {
    GrImageDisplay(0, 0, GrImageFromPattern(p));
    getkey();
  }
  if ( p256 ) {
    GrImageDisplay(300, 300, GrImageFromPattern(p256));
    getkey();
  }
  if ( p ) grx_pattern_free(p);
  if ( p256 ) grx_pattern_free(p256);
  GrSaveBmpImage("save.bmp", NULL, 0, 0, 400, 400);
  GrUnloadBmpImage(bmp);
  GrUnloadBmpImage(bmp256);
  return 0;
};

