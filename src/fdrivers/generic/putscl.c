/*
** GENERIC/PUTSCL.C ---- generic putscanline routine
**
** Copyright (c) 1998 Hartmut Schirmer
*/

void putscanline(int x,int y,int w,
                 const GrColor far *scl, GrColor op )
{
   GrColor skipc;
   GRX_ENTER();
   skipc = op ^ GrIMAGE;
   op &= GrCMODEMASK;
   for ( w += x; x < w; ++x) {
     GrColor c = *(scl++);
     if (c != skipc) drawpixel(x,y,(c|op));
   }
   GRX_LEAVE();
}
