/**
 ** RBLIT_14.H ---- ram to ram blit support functions for 1bpp and
 **                 4bpp RAM frame drivers
 **
 ** Copyright (c) 1998 Hartmut Schirmer
 **/

void _GR_rblit_14(GrFrame *dst,int dx,int dy,
                  GrFrame *src,int x,int y,int w,int h,
                  GrColor op, int planes, _GR_blitFunc bitblt);
