/**
 ** GENERIC/BLOCK.C ---- generic (=slow) rectangle fill routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/
  
void drawblock(int x,int y,int w,int h,long c)
{
        h += y;
        do { drawhline(x,y,w,c); } while(++y != h);
}

