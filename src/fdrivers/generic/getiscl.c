/*
** GENERIC/GENGISCL.C ---- generic getindexscanline routine
**
** Copyright (c) 1998 Hartmut Schirmer
*/


GrColor far *getindexedscanline(GrFrame *c,
                                int x,int y,int w,
                                int *indx          )
{
   GrColor far *pixels;
   GrColor far *p;
   GRX_ENTER();
   p = pixels = _GrTempBufferAlloc(sizeof(GrColor) * (w+1));
   if (pixels) {
     if (indx) {
       int i, oldx = -1;
       GrColor col = 0;
       for (i=0; i < w; ++i) {
         int xx = x+indx[i];
         if (oldx != xx) {
           oldx = xx;
           col = readpixel(c,xx,y);
         }
         *(p++) = col;
       }
     } else {
       for ( ; w > 0; --w)
         *(p++) = readpixel(c,x++,y);
     }
   }
   GRX_RETURN(pixels);
}
/* will return an array of pixel values pv[] read from frame   */
/*    if indx == NULL: pv[i=0..w-1] = readpixel(x+i,y)         */
/*    else             pv[i=0..w-1] = readpixel(x+indx[i],y)   */
