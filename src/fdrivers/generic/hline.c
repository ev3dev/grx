/**
 ** GENERIC/HLINE.C ---- generic (=slow) scan line fill routine
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

void drawhline(int x,int y,int w,GrColor c)
{
        GRX_ENTER();
        w += x;
        do { drawpixel(x,y,c); } while(++x != w);
        GRX_LEAVE();
}

