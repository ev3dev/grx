/**
 ** TEXT.H ---- some shared definitions
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **/

typedef void (*TextDrawBitmapFunc)(int x,int y,int w,int h,int ox, int oy,
                                   char far *bmp,int pitch,int start,
                                   GrColor fg,GrColor bg,GrPattern *p);

void _GrDrawString(void *text,int length,int x,int y,
                   GrTextOption *opt, GrPattern *p, TextDrawBitmapFunc dbm);
