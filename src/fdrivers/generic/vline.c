/**
 ** GENERIC/BITBLT.C ---- generic (=slow) vertical line draw routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
  
void drawvline(int x,int y,int h,long c)
{
        h += y;
        do { drawpixel(x,y,c); } while(++y != h);
}

